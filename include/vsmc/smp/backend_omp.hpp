#ifndef VSMC_SMP_BACKEND_OMP_HPP
#define VSMC_SMP_BACKEND_OMP_HPP

#include <vsmc/smp/base.hpp>
#include <omp.h>

namespace vsmc {

/// \brief Particle::value_type subtype using OpenMP
/// \ingroup SMP
template <std::size_t Dim, typename T>
class StateOMP : public StateBase<Dim, T>
{
    public :

    typedef typename internal::OMPSizeTypeTrait<
        typename StateBase<Dim, T>::size_type>::type size_type;

    explicit StateOMP (size_type N) : StateBase<Dim, T>(N) {}

    size_type size () const
    {
        return static_cast<size_type>(StateBase<Dim, T>::size());
    }

    template <typename IntType>
    void copy (size_type N, const IntType *copy_from)
    {
        VSMC_RUNTIME_ASSERT_STATE_COPY_SIZE_MISMATCH(OMP);

#pragma omp parallel for default(shared)
        for (size_type to = 0; to < N; ++to)
            this->copy_particle(copy_from[to], to);
    }
}; // class StateOMP

/// \brief Sampler<T>::init_type subtype using OpenMP
/// \ingroup SMP
template <typename T, typename Derived>
class InitializeOMP : public InitializeBase<T, Derived>
{
    public :

    std::size_t operator() (Particle<T> &particle, void *param)
    {
        typedef typename internal::OMPSizeTypeTrait<
            typename Particle<T>::size_type>::type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->initialize_param(particle, param);
        this->pre_processor(particle);
        std::size_t accept = 0;
#pragma omp parallel for reduction(+ : accept) default(shared)
        for (size_type i = 0; i < N; ++i)
            accept += this->initialize_state(SingleParticle<T>(i, &particle));
        this->post_processor(particle);

        return accept;
    }

    protected :

    InitializeOMP () {}
    InitializeOMP (const InitializeOMP<T, Derived> &) {}
    InitializeOMP<T, Derived> &operator=
        (const InitializeOMP<T, Derived> &) {return *this;}
    ~InitializeOMP () {}
}; // class InitializeOMP

/// \brief Sampler<T>::move_type subtype using OpenMP
/// \ingroup SMP
template <typename T, typename Derived>
class MoveOMP : public MoveBase<T, Derived>
{
    public :

    std::size_t operator() (std::size_t iter, Particle<T> &particle)
    {
        typedef typename internal::OMPSizeTypeTrait<
            typename Particle<T>::size_type>::type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        std::size_t accept = 0;
#pragma omp parallel for reduction(+ : accept) default(shared)
        for (size_type i = 0; i < N; ++i)
            accept += this->move_state(iter, SingleParticle<T>(i, &particle));
        this->post_processor(iter, particle);

        return accept;
    }

    protected :

    MoveOMP () {}
    MoveOMP (const MoveOMP<T, Derived> &) {}
    MoveOMP<T, Derived> &operator=
        (const MoveOMP<T, Derived> &) {return *this;}
    ~MoveOMP () {}
}; // class MoveOMP

/// \brief Monitor<T>::eval_type subtype using OpenMP
/// \ingroup SMP
template <typename T, typename Derived>
class MonitorEvalOMP : public MonitorEvalBase<T, Derived>
{
    public :

    void operator() (std::size_t iter, std::size_t dim,
            const Particle<T> &particle, double *res)
    {
        typedef typename internal::OMPSizeTypeTrait<
            typename Particle<T>::size_type>::type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
#pragma omp parallel for default(shared)
        for (size_type i = 0; i < N; ++i)
            this->monitor_state(iter, dim,
                    ConstSingleParticle<T>(i, &particle), res + i * dim);
        this->post_processor(iter, particle);
    }

    protected :

    MonitorEvalOMP () {}
    MonitorEvalOMP (const MonitorEvalOMP<T, Derived> &) {}
    MonitorEvalOMP<T, Derived> &operator=
        (const MonitorEvalOMP<T, Derived> &) {return *this;}
    ~MonitorEvalOMP () {}
}; // class MonitorEvalOMP

/// \brief Path<T>::eval_type subtype using OpenMP
/// \ingroup SMP
template <typename T, typename Derived>
class PathEvalOMP : public PathEvalBase<T, Derived>
{
    public :

    double operator() (std::size_t iter, const Particle<T> &particle,
            double *res)
    {
        typedef typename internal::OMPSizeTypeTrait<
            typename Particle<T>::size_type>::type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
#pragma omp parallel for default(shared)
        for (size_type i = 0; i < N; ++i)
            res[i] = this->path_state(iter,
                    ConstSingleParticle<T>(i, &particle));
        this->post_processor(iter, particle);

        return this->path_grid(iter, particle);
    }

    protected :

    PathEvalOMP () {}
    PathEvalOMP (const PathEvalOMP<T, Derived> &) {}
    PathEvalOMP<T, Derived> &operator=
        (const PathEvalOMP<T, Derived> &) {return *this;}
    ~PathEvalOMP () {}
}; // class PathEvalOMP

} // namespace vsmc

#endif // VSMC_SMP_BACKEND_OMP_HPP