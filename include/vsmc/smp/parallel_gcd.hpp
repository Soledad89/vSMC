#ifndef VSMC_SMP_PARALLEL_GCD_HPP
#define VSMC_SMP_PARALLEL_GCD_HPP

#include <vsmc/internal/common.hpp>
#include <vsmc/smp/base.hpp>
#include <dispatch/dispatch.h>

namespace vsmc {

namespace gcd {

/// \brief Apple GCD informations
/// \ingroup GCD
class DispatchQueue
{
    public :

    static DispatchQueue &instance ()
    {
        static DispatchQueue queue;

        return queue;
    }

    dispatch_queue_t queue () const
    {
        return queue_;
    }

    void queue (dispatch_queue_t new_queue)
    {
        queue_ = new_queue;
    }

    private :

    dispatch_queue_t queue_;

    DispatchQueue () : queue_(
            dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)) {}

    DispatchQueue (const DispatchQueue &);
    DispatchQueue &operator= (const DispatchQueue &);
}; // class DispatchQueue

} // namespace vsmc::gcd

#if !VSMC_HAS_CXX11_ALIAS_TEMPLATES
/// \brief Particle::value_type subtype
/// \ingroup GCD
template <unsigned Dim, typename T>
class StateGCD : public StateBase<Dim, T>
{
    public :

    typedef StateBase<Dim, T> state_base_type;
    typedef typename state_base_type::size_type size_type;
    typedef typename state_base_type::state_type state_type;

    explicit StateGCD (size_type N) : StateBase<Dim, T>(N) {}
}; // class StateGCD
#endif // VSMC_HAS_CXX11_ALIAS_TEMPLATES

/// \brief Sampler<T>::init_type subtype
/// \ingroup GCD
template <typename T, typename Derived>
class InitializeGCD : public InitializeBase<T, Derived>
{
    public :

    typedef InitializeBase<T, Derived> initialize_base_type;
    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    unsigned operator() (Particle<T> &particle, void *param)
    {
        this->initialize_param(particle, param);
        this->pre_processor(particle);
        accept_.resize(particle.size());
        work_param_ wp = {this, &particle, &accept_[0]};
        dispatch_apply_f(particle.size(),
                gcd::DispatchQueue::instance().queue(),
                (void *) &wp, work_);
        this->post_processor(particle);

        return std::accumulate(accept_.begin(), accept_.end(),
                static_cast<unsigned>(0));
    }

    protected :

    InitializeGCD () {}
    InitializeGCD (const InitializeGCD<T, Derived> &) {}
    InitializeGCD<T, Derived> &operator=
        (const InitializeGCD<T, Derived> &) {return *this;}
    ~InitializeGCD () {}

    private :

    std::vector<unsigned> accept_;

    struct work_param_
    {
        InitializeGCD<T, Derived> *const dispatcher;
        Particle<T> *const particle;
        unsigned *const accept;
    };

    static void work_ (void *wp, std::size_t i)
    {
        const work_param_ *const wptr =
            reinterpret_cast<const work_param_ *>(wp);
        wptr->accept[i] = wptr->dispatcher->initialize_state(
                SingleParticle<T>(static_cast<size_type>(i), wptr->particle));
    }
}; // class InitializeGCD

/// \brief Sampler<T>::move_type subtype
/// \ingroup GCD
template <typename T, typename Derived>
class MoveGCD : public MoveBase<T, Derived>
{
    public :

    typedef MoveBase<T, Derived> move_base_type;
    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    unsigned operator() (unsigned iter, Particle<T> &particle)
    {
        this->pre_processor(iter, particle);
        accept_.resize(particle.size());
        work_param_ wp = {this, &particle, &accept_[0], iter};
        dispatch_apply_f(particle.size(),
                gcd::DispatchQueue::instance().queue(),
                (void *) &wp, work_);
        this->post_processor(iter, particle);

        return std::accumulate(accept_.begin(), accept_.end(),
                static_cast<unsigned>(0));
    }

    protected :

    MoveGCD () {}
    MoveGCD (const MoveGCD<T, Derived> &) {}
    MoveGCD<T, Derived> &operator=
        (const MoveGCD<T, Derived> &) {return *this;}
    ~MoveGCD () {}

    private :

    std::vector<unsigned> accept_;

    struct work_param_
    {
        MoveGCD<T, Derived> *const dispatcher;
        Particle<T> *const particle;
        unsigned *const accept;
        unsigned iter;
    };

    static void work_ (void *wp, std::size_t i)
    {
        const work_param_ *const wptr =
            reinterpret_cast<const work_param_ *>(wp);
        wptr->accept[i] = wptr->dispatcher->move_state(wptr->iter,
                SingleParticle<T>(static_cast<size_type>(i), wptr->particle));
    }
}; // class MoveGCD

/// \brief Monitor<T>::eval_type subtype
/// \ingroup GCD
template <typename T, typename Derived>
class MonitorEvalGCD : public MonitorEvalBase<T, Derived>
{
    public :

    typedef MonitorEvalBase<T, Derived> monitor_eval_base_type;
    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    void operator() (unsigned iter, unsigned dim, const Particle<T> &particle,
            double *res)
    {
        this->pre_processor(iter, particle);
        work_param_ wp = {this, &particle, res, iter, dim};
        dispatch_apply_f(particle.size(),
                gcd::DispatchQueue::instance().queue(),
                (void *) &wp, work_);
        this->post_processor(iter, particle);
    }

    protected :

    MonitorEvalGCD () {}
    MonitorEvalGCD (const MonitorEvalGCD<T, Derived> &) {}
    MonitorEvalGCD<T, Derived> &operator=
        (const MonitorEvalGCD<T, Derived> &) {return *this;}
    ~MonitorEvalGCD () {}

    private :

    struct work_param_
    {
        MonitorEvalGCD<T, Derived> *const dispatcher;
        const Particle<T> *const particle;
        double *const res;
        unsigned iter;
        unsigned dim;
    };

    static void work_ (void *wp, std::size_t i)
    {
        const work_param_ *const wptr =
            reinterpret_cast<const work_param_ *>(wp);
        wptr->dispatcher->monitor_state(wptr->iter, wptr->dim,
                ConstSingleParticle<T>(
                    static_cast<size_type>(i), wptr->particle),
                wptr->res + i * wptr->dim);
    }
}; // class MonitorEvalGCD

/// \brief Path<T>::eval_type subtype
/// \ingroup GCD
template <typename T, typename Derived>
class PathEvalGCD : public PathEvalBase<T, Derived>
{
    public :

    typedef PathEvalBase<T, Derived> path_eval_base_type;
    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    double operator() (unsigned iter, const Particle<T> &particle, double *res)
    {
        this->pre_processor(iter, particle);
        work_param_ wp = {this, &particle, res, iter};
        dispatch_apply_f(particle.size(),
                gcd::DispatchQueue::instance().queue(),
                (void *) &wp, work_);
        this->post_processor(iter, particle);

        return this->path_width(iter, particle);
    }

    protected :

    PathEvalGCD () {}
    PathEvalGCD (const PathEvalGCD<T, Derived> &) {}
    PathEvalGCD<T, Derived> &operator=
        (const PathEvalGCD<T, Derived> &) {return *this;}
    ~PathEvalGCD () {}

    private :

    struct work_param_
    {
        PathEvalGCD<T, Derived> *const dispatcher;
        const Particle<T> *const particle;
        double *const res;
        unsigned iter;
    };

    static void work_ (void *wp, std::size_t i)
    {
        const work_param_ *const wptr =
            reinterpret_cast<const work_param_ *>(wp);
        wptr->res[i] = wptr->dispatcher->path_state(wptr->iter,
                ConstSingleParticle<T>(
                    static_cast<size_type>(i), wptr->particle));
    }
}; // class PathEvalGCD

}

#endif // VSMC_SMP_PARALLEL_GCD_HPP
