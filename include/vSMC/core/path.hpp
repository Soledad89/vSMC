#ifndef V_SMC_CORE_PATH_HPP
#define V_SMC_CORE_PATH_HPP

#include <vector>
#include <cstddef>
#include <boost/function.hpp>
#include <Eigen/Dense>
#include <vSMC/core/particle.hpp>
#include <vSMC/internal/version.hpp>

namespace vSMC {

/// \brief Monitor for path sampling
///
/// Path record the path sampling integrand and width when the Sampler
/// progress. It is also used for retrieve information of path sampling after
/// the SMC iterations.
template <typename T>
class Path
{
    public :

    /// The type of path sampling integral functor
    typedef boost::function<double (std::size_t, Particle<T> &, double *)>
        integral_type;
    /// The type of the index vector
    typedef std::vector<std::size_t> index_type;
    /// The type of the integrand vector
    typedef std::vector<double> integrand_type;
    /// The type of the width vector
    typedef std::vector<double> width_type;
    /// The type of the grid vector
    typedef std::vector<double> grid_type;

    /// \brief Construct a Path with an integral function
    ///
    /// \param integral The functor used to compute the integrands
    Path (const integral_type &integral = NULL) : integral_(integral) {}

    /// \brief Copy constructor
    ///
    /// \param path The Path to by copied
    Path (const Path<T> &path) : integral_(path.integral_),
        index_(path.index_), integrand_(path.integrand_),
        width_(path.width_), grid_(path.grid_) {}

    /// \brief Assignment operator
    ///
    /// \param path The Path to be assigned
    /// \return The Path after assignemnt
    Path<T> & operator= (const Path<T> &path)
    {
        if (&path != this) {
            integral_ = path.integral_;
            index_ = path.index_;
            integrand_ = path.integrand_;
            width_ = path.width_;
            grid_ = path.grid_;
        }

        return *this;
    }

    /// \brief Size of records
    ///
    /// \return The number of iterations recorded
    index_type::size_type iter_size () const
    {
        return index_.size();
    }

    /// \brief Set the integral functor
    ///
    /// \param integral The functor used to compute the integrands
    void integral (const integral_type &integral)
    {
        integral_ = integral;
    }

    /// \brief Test if the path is empty
    ///
    /// \return \b true if the path is empty
    bool empty () const
    {
        return integral_.empty();
    }

    /// \brief Iteration index
    ///
    /// \return A const reference to the index
    const index_type &index () const
    {
        return index_;
    }

    /// \brief Record of path sampling integrand
    ///
    /// \return A const reference to the integrand
    const integrand_type &integrand () const
    {
        return integrand_;
    }

    /// \brief Record of path sampling width
    ///
    /// \return A const reference to the width
    const width_type &width () const
    {
        return width_;
    }

    /// \brief Record of path sampling grid
    ///
    /// \return A const reference to the grid
    const grid_type &grid () const
    {
        return grid_;
    }

    /// \brief Evaluate the integration
    ///
    /// \param iter The iteration number
    /// \param particle The particle set to be operated on by eval()
    ///
    /// \note The integral function has to be set through either the
    /// constructor or integral() to a non-NULL value before calling eval().
    /// Otherwise runtime_error exception will be raised when calling eval().
    ///
    /// \see Documentation for Boost::function
    void eval (std::size_t iter, Particle<T> &particle)
    {
        buffer_.resize(particle.size());
        width_.push_back(integral_(iter, particle, buffer_.data()));
        integrand_.push_back(particle.weight().dot(buffer_));
        index_.push_back(iter);
        grid_.push_back(grid_.size() ?
                grid_.back() + width_.back() : width_.back());
    }

    /// \brief Path sampling estimate of normalizing constant
    ///
    /// \return The Path sampling normalzing constant estimate
    double zconst () const
    {
	double sum = 0;
	for (std::size_t i = 1; i != iter_size(); ++i)
            sum += 0.5 * width_[i] * (integrand_[i-1] + integrand_[i]);

        return sum;
    }

    /// \brief Clear all recorded data
    void clear ()
    {
        index_.clear();
        integrand_.clear();
        width_.clear();
        grid_.clear();
    }

    private :

    Eigen::VectorXd buffer_;
    integral_type integral_;
    index_type index_;
    integrand_type integrand_;
    width_type width_;
    grid_type grid_;
}; // class PathSampling

} // namespace vSMC

#endif // V_SMC_CORE_PATH_HPP
