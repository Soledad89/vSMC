#ifndef V_SMC_SAMPLER_HPP
#define V_SMC_SAMPLER_HPP

#include <vSMC/config.hpp>

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <mkl_cblas.h>
#include <boost/function.hpp>
#include <vDist/rng/gsl.hpp>
#include <vDist/tool/buffer.hpp>
#include <vSMC/monitor.hpp>
#include <vSMC/particle.hpp>
#include <vSMC/path.hpp>

namespace vSMC {

template <typename T>
class Sampler
{
    public :

    /// The type of initialization functor
    typedef boost::function<std::size_t
        (Particle<T> &, void *)> init_type;
    /// The type of move functor
    typedef boost::function<std::size_t
        (std::size_t, Particle<T> &)> move_type;
    /// The type of importance sampling integral functor
    typedef boost::function<void
        (std::size_t, Particle<T> &, double *, void *)> integral_type;

    /// \brief Sampler does not have a default constructor
    ///
    /// \param N The number of particles
    /// \param init The functor used to initialize the particles
    /// \param move The functor used to move the particles and weights
    /// \param mcmc The functor used to perform MCMC move
    /// \param scheme The resampling scheme. See ResampleScheme
    /// \param threshold The threshold for performing resampling
    /// \param seed The seed for the reampling RNG. See documentation of vDist
    /// \param brng The basic RNG for resampling RNG. See documentation of GSL
    Sampler (
            std::size_t N,
            const init_type &init,
            const move_type &move,
            const move_type &mcmc = NULL,
            ResampleScheme scheme = RESIDUAL,
            double threshold = 0.5,
            const int seed = V_DIST_SEED,
            const gsl_rng_type *brng = V_DIST_GSL_BRNG) :
        initialized_(false), init_(init), move_(move), mcmc_(mcmc),
        rng_(seed, brng), scheme_(scheme), threshold_(threshold * N),
        particle_(N), iter_num_(0), show_progress_(false) {}

    /// \brief Size of the particle set
    ///
    /// \return The number of particles
    std::size_t size () const
    {
        return particle_.size();
    }

    /// \brief Size of records
    ///
    /// \return The number of iterations recorded (including the
    /// initialization step)
    std::size_t iter_size () const
    {
        return ess_.size();
    }

    /// \brief ESS
    ///
    /// \return The ESS value of the latest iteration
    double ess () const
    {
        return ess_.back();
    }

    /// \brief ESS history
    ///
    /// \return A const reference to the history of ESS
    const std::vector<double> &ess_history () const
    {
        return ess_;
    }

    /// \brief ESS history
    ///
    /// \param first An iterator point to where writing starts
    template<typename OIter>
    void ess_history (OIter first) const
    {
        for (std::vector<double>::const_iterator iter = ess_.begin();
                iter != ess_.end(); ++iter)
            *first++ = *iter;
    }

    /// \brief Indicator of resampling
    ///
    /// \return A bool value, \b true if the latest iteration was resampled
    bool resampled () const
    {
        return resampled_.back();
    }

    /// \brief Resampling history
    ///
    /// \return A const reference to the history of resampling
    const std::vector<bool> &resampled_history () const
    {
        return resampled_;
    }

    /// \brief Resampling history
    ///
    /// \param first An iterator point to where writing starts
    template<typename OIter>
    void resampled_history (OIter first) const
    {
        for (std::vector<bool>::const_iterator iter = resampled_.begin();
                iter != resampled_.end(); ++iter)
            *first++ = *iter;
    }

    /// \brief Accept count
    ///
    /// \return The accept count of the latest iteration
    std::size_t accept () const
    {
        return accept_.back();
    }

    /// \brief Accept count history
    ///
    /// \return A const reference to the history of accept count
    const std::vector<std::size_t> &accept_history () const
    {
        return accept_;
    }

    /// \brief Accept count history
    ///
    /// \param first An iterator point to where writing starts
    template<typename OIter>
    void accept_history (OIter first) const
    {
        for (std::vector<std::size_t>::const_iterator iter = accept_.begin();
                iter != accept_.end(); ++iter)
            *first++ = *iter;
    }

    /// \brief Read and write access to the particle set
    ///
    /// \return A reference to the latest particle set
    ///
    /// \note The Sampler class guarantee that during the life type of the
    /// object, the reference returned by this member will no be a dangle
    /// handler.
    Particle<T> &particle ()
    {
        return particle_;
    }

    /// \brief Read only access to the particle set
    ///
    /// \return A const reference to the latest particle set.
    const Particle<T> &particle () const
    {
        return particle_;
    }

    /// \brief Initialize the particle set
    ///
    /// \param param Additional parameters passed to the initialization
    /// functor
    void initialize (void *param = NULL)
    {
        ess_.clear();
        resampled_.clear();
        accept_.clear();
        path_.clear();

        for (typename std::map<std::string, Monitor<T> >::iterator
                imap = monitor_.begin(); imap != monitor_.end(); ++imap)
            imap->second.clear();

        iter_num_ = 0;
        accept_.push_back(init_(particle_, param));
        post_move();
        particle_.reset_zconst();

        initialized_ = true;
    }

    /// \brief Perform iteration
    void iterate ()
    {
        if (!initialized_) {
            throw std::runtime_error(
                    "ERROR: vSMC::Sampler::iterate: "
                    "Sampler has not been initialized yet");
        }

        ++iter_num_;
        accept_.push_back(move_(iter_num_, particle_));
        if (mcmc_)
            accept_.back() = mcmc_(iter_num_, particle_);
        post_move();
    }

    /// \brief Perform iteration
    ///
    /// \param n The number of iterations to be performed
    void iterate (std::size_t n)
    {
        for (std::size_t i = 0; i != n; ++i)
            iterate();
    }

    /// \brief Perform importance sampling integration
    ///
    /// \param integral The functor used to compute the integrands
    double integrate (typename Monitor<T>::integral_type integral)
    {
        buffer_.resize(size());
        integral(iter_num_, particle_, buffer_);

        return cblas_ddot(size(), particle_.weight_ptr(), 1, buffer_, 1);
    }

    /// \brief Perform importance sampling integration
    ///
    /// \param integral The functor used to compute the integrands
    /// \param param Additional parameters passed to the integral functor
    double integrate (integral_type integral, void *param) const
    {
        buffer_.resize(size());
        integral(iter_num_, particle_, buffer_, param);

        return cblas_ddot(size(), particle_.weight_ptr(), 1, buffer_, 1);
    }

    /// \brief Add a monitor, similar to \b monitor in \b BUGS
    ///
    /// \param name The name of the monitor
    /// \param integral The functor used to compute the integrands
    void monitor (const std::string &name,
            const typename Monitor<T>::integral_type &integral)
    {
        monitor_.insert(std::make_pair(name, Monitor<T>(integral)));
        monitor_name_.insert(name);
    }

    /// \brief Read and write access to a named monitor through iterator
    ///
    /// \param name The name of the monitor
    /// \return An iterator point to the monitor for the given name
    typename std::map<std::string, Monitor<T> >::iterator
        monitor (const std::string &name)
    {
        return monitor_.find(name);
    }

    /// \brief Read only access to a named monitor through iterator
    ///
    /// \param name The name of the monitor
    /// \return An const_iterator point to the monitor for the given name
    typename std::map<std::string, Monitor<T> >::const_iterator
        monitor (const std::string &name) const
    {
        return monitor_.find(name);
    }

    /// \brief Read and write access to all monitors
    ///
    /// \return A reference to monitors
    std::map<std::string, Monitor<T> > &monitor ()
    {
        return monitor_;
    }

    /// \brief Read only access to all monitors
    ///
    /// \return A const reference to monitors
    const std::map<std::string, Monitor<T> > &monitor () const
    {
        return monitor_;
    }

    /// \brief Erase a named monitor
    ///
    /// \param name The name of the monitor
    void erase_monitor (const std::string &name)
    {
        monitor_.erase(name);
        monitor_name_.erase(name);
    }

    /// \brief Erase (clear) all monitors
    void clear_monitor ()
    {
        monitor_.clear();
        monitor_name_.clear();
    }

    /// \brief Read and write access to the Path sampling monitor
    ///
    /// \return A reference to the Path sampling monitor
    Path<T> &path ()
    {
        return path_;
    }

    /// \brief Read only access to the Path sampling monitor
    ///
    /// \return A const reference to the Path sampling monitor
    const Path<T> &path () const
    {
        return path_;
    }

    /// \brief Set the path sampling integral
    ///
    /// \param integral The functor used to compute the integrands
    ///
    /// \note Set integral = NULL will stop path sampling recording
    void path_sampling (const typename Path<T>::integral_type &integral)
    {
        path_.integral(integral);
    }

    /// \brief Path sampling estimate of normalizing constant
    ///
    /// \return The log ratio of normalizing constants
    double path_sampling_zconst () const
    {
        return path_.zconst();
    }

    /// \brief Toggle whether or not record SMC normalizing constant
    ///
    /// \param estimate_zconst Start estimating normalzing constant if true.
    void zconst (bool estimate_zconst)
    {
        particle_.zconst(estimate_zconst);
    }

    /// \brief SMC estimate of normalizing constant
    ///
    /// \return The SMC normalizng constant estimate
    double zconst () const
    {
        return particle_.zconst();
    }

    /// \brief Print the history of the sampler
    ///
    /// \param output The ostream to which the contents are printed
    /// \param print_header Print header if \b true
    void print (std::ostream &output = std::cout,
            bool print_header = true) const
    {
        print(output, print_header, !path_.index().empty(), monitor_name_);
    }

    /// \brief Print the history of the sampler
    ///
    /// \param output The ostream to which the contents are printed
    /// \param print_path Print path sampling history if \b true
    /// \param print_monitor A set of monitor names to be printed
    /// \param print_header Print header if \b true
    void print (std::ostream &output,
            bool print_header, bool print_path,
            const std::set<std::string> &print_monitor) const
    {
        if (print_header) {
            output << "iter\tESS\tresample\taccept\t";
            if (print_path)
                output << "path.integrand\tpath.width\tpath.grid\t";
        }

        std::vector<std::size_t>::const_iterator iter_path_index
            = path_.index().begin();
        std::vector<double>::const_iterator iter_path_integrand
            = path_.integrand().begin();
        std::vector<double>::const_iterator iter_path_width
            = path_.width().begin();
        std::vector<double>::const_iterator iter_path_grid
            = path_.grid().begin();

        std::vector<bool> monitor_index_empty;
        std::vector<std::vector<std::size_t>::const_iterator>
            iter_monitor_index;
        std::vector<std::vector<double>::const_iterator>
            iter_monitor_record;
        for (typename std::map<std::string, Monitor<T> >::const_iterator
                imap = monitor_.begin(); imap != monitor_.end(); ++imap) {
            if (print_monitor.count(imap->first)) {
                monitor_index_empty.push_back(imap->second.index().empty());
                iter_monitor_index.push_back(imap->second.index().begin());
                iter_monitor_record.push_back(imap->second.record().begin());
                if (print_header)
                    output << imap->first << '\t';
            }
        }

        if (print_header)
            output << '\n';

        for (std::size_t i = 0; i != iter_size(); ++i) {
            output
                << i << '\t' << ess_[i] / size()
                << '\t' << resampled_[i]
                << '\t' << static_cast<double>(accept_[i]) / size();

            if (print_path) {
                if (!path_.index().empty() && *iter_path_index == i) {
                    output
                        << '\t' << *iter_path_integrand++
                        << '\t' << *iter_path_width++
                        << '\t' << *iter_path_grid++;
                    ++iter_path_index;
                } else {
                    output << '\t' << '.' << '\t' << '.' << '\t' << '.';
                }
            }

            for (std::size_t m = 0; m != monitor_index_empty.size(); ++m) {
                if (!monitor_index_empty[m] && *iter_monitor_index[m] == i) {
                    output << '\t' << *iter_monitor_record[m]++;
                    ++iter_monitor_index[m];
                } else {
                    output << '\t' << '.';
                }
            }

            output << '\n';
        }
    }

    private :

    /// Initialization indicator
    bool initialized_;

    /// Initialization and movement
    init_type init_;
    move_type move_;
    move_type mcmc_;

    /// Resampling
    vDist::RngGSL rng_;
    ResampleScheme scheme_;
    double threshold_;

    /// Particle sets
    Particle<T> particle_;
    std::size_t iter_num_;
    std::vector<double> ess_;
    std::vector<bool> resampled_;
    std::vector<std::size_t> accept_;

    /// Monte Carlo estimation by integration
    vDist::tool::Buffer<double> buffer_;
    std::map<std::string, Monitor<T> > monitor_;
    std::set<std::string> monitor_name_;

    /// Path sampling
    Path<T> path_;

    /// Whether to show prograss while iterating
    bool show_progress_;

    void post_move ()
    {
        ess_.push_back(particle_.ess());
        particle_.resampled(ess_.back() < threshold_);
        resampled_.push_back(particle_.resampled());
        if (particle_.resampled())
            particle_.resample(scheme_, rng_.get_rng());

        if (!path_.empty())
            path_.eval(iter_num_, particle_);

        for (typename std::map<std::string, Monitor<T> >::iterator
                imap = monitor_.begin(); imap != monitor_.end(); ++imap) {
            if (!imap->second.empty())
                imap->second.eval(iter_num_, particle_);
        }

        if (show_progress_) {
            if (iter_num_)
                std::cerr << '.';
            else
                std::cerr << '*';
            std::cerr.flush();
        }
    }
}; // class Sampler

} // namespace vSMC

namespace std {

template<typename T>
std::ostream & operator<< (std::ostream &output,
        const vSMC::Sampler<T> &sampler)
{
    sampler.print(output);

    return output;
}

} // namespace std

#endif // V_SMC_SAMPLER_HPP
