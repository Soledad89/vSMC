#ifndef VSMC_SMP_BACKEND_CILK_HPP
#define VSMC_SMP_BACKEND_CILK_HPP

#include <vsmc/smp/base.hpp>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>

namespace vsmc {

/// \brief Particle::value_type subtype using Intel Cilk Plus
/// \ingroup SMP
template <typename BaseState>
class StateCILK : public BaseState
{
    public :

    typedef typename traits::SizeTypeTrait<BaseState>::type size_type;

    explicit StateCILK (size_type N) : BaseState(N) {}

    template <typename IntType>
    void copy (size_type N, const IntType *copy_from)
    {
        VSMC_RUNTIME_ASSERT_STATE_COPY_SIZE_MISMATCH(CILK);

        cilk_for (size_type to = 0; to != N; ++to)
            this->copy_particle(copy_from[to], to);
    }
}; // class StateCILK

/// \brief Sampler<T>::init_type subtype using Intel Cilk Plus
/// \ingroup SMP
template <typename T, typename Derived>
class InitializeCILK : public InitializeBase<T, Derived>
{
    public :

    std::size_t operator() (Particle<T> &particle, void *param)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->initialize_param(particle, param);
        this->pre_processor(particle);
        cilk::reducer_opadd<std::size_t> accept;
        cilk_for (size_type i = 0; i != N; ++i)
            accept += this->initialize_state(SingleParticle<T>(i, &particle));
        this->post_processor(particle);

        return accept.get_value();
    }

    protected :

    InitializeCILK () {}
    InitializeCILK (const InitializeCILK<T, Derived> &) {}
    InitializeCILK<T, Derived> &operator=
        (const InitializeCILK<T, Derived> &) {return *this;}
    ~InitializeCILK () {}
}; // class InitializeCILK

/// \brief Sampler<T>::move_type subtype using Intel Cilk Plus
/// \ingroup SMP
template <typename T, typename Derived>
class MoveCILK : public MoveBase<T, Derived>
{
    public :


    std::size_t operator() (std::size_t iter, Particle<T> &particle)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        cilk::reducer_opadd<std::size_t> accept;
        cilk_for (size_type i = 0; i != N; ++i)
            accept += this->move_state(iter, SingleParticle<T>(i, &particle));
        this->post_processor(iter, particle);

        return accept.get_value();
    }

    protected :

    MoveCILK () {}
    MoveCILK (const MoveCILK<T, Derived> &) {}
    MoveCILK<T, Derived> &operator=
        (const MoveCILK<T, Derived> &) {return *this;}
    ~MoveCILK () {}
}; // class MoveCILK

/// \brief Monitor<T>::eval_type subtype using Intel Cilk Plus
/// \ingroup SMP
template <typename T, typename Derived>
class MonitorEvalCILK : public MonitorEvalBase<T, Derived>
{
    public :


    void operator() (std::size_t iter, std::size_t dim,
            const Particle<T> &particle, double *res)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        cilk_for (size_type i = 0; i != N; ++i) {
            this->monitor_state(iter, dim,
                    ConstSingleParticle<T>(i, &particle), res + i * dim);
        }
        this->post_processor(iter, particle);
    }

    protected :

    MonitorEvalCILK () {}
    MonitorEvalCILK (const MonitorEvalCILK<T, Derived> &) {}
    MonitorEvalCILK<T, Derived> &operator=
        (const MonitorEvalCILK<T, Derived> &) {return *this;}
    ~MonitorEvalCILK () {}
}; // class MonitorEvalCILK

/// \brief Path<T>::eval_type subtype using Intel Cilk Plus
/// \ingroup SMP
template <typename T, typename Derived>
class PathEvalCILK : public PathEvalBase<T, Derived>
{
    public :

    double operator() (std::size_t iter, const Particle<T> &particle,
            double *res)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        cilk_for (size_type i = 0; i != N; ++i) {
            res[i] = this->path_state(iter,
                    ConstSingleParticle<T>(i, &particle));
        }
        this->post_processor(iter, particle);

        return this->path_grid(iter, particle);
    }

    protected :

    PathEvalCILK () {}
    PathEvalCILK (const PathEvalCILK<T, Derived> &) {}
    PathEvalCILK<T, Derived> &operator=
        (const PathEvalCILK<T, Derived> &) {return *this;}
    ~PathEvalCILK () {}
}; // class PathEvalCILK

} // namespace vsmc

#endif // VSMC_SMP_BACKEND_CILK_HPP
