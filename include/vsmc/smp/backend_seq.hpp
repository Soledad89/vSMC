#ifndef VSMC_SMP_BACKEND_SEQ_HPP
#define VSMC_SMP_BACKEND_SEQ_HPP

#include <vsmc/smp/base.hpp>

namespace vsmc {

/// \brief Particle::value_type subtype
/// \ingroup Sequential
template <std::size_t Dim, typename T>
class StateSEQ : public StateBase<Dim, T>
{
    public :

    typedef typename StateBase<Dim, T>::size_type size_type;

    explicit StateSEQ (size_type N) : StateBase<Dim, T>(N) {}
}; // class StateSEQ

/// \brief Sampler<T>::init_type subtype
/// \ingroup Sequential
template <typename T, typename Derived>
class InitializeSEQ : public InitializeBase<T, Derived>
{
    public :


    std::size_t operator() (Particle<T> &particle, void *param)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->initialize_param(particle, param);
        this->pre_processor(particle);
        std::size_t accept = 0;
        for (size_type i = 0; i != N; ++i)
            accept += this->initialize_state(SingleParticle<T>(i, &particle));
        this->post_processor(particle);

        return accept;
    }

    protected :

    InitializeSEQ () {}
    InitializeSEQ (const InitializeSEQ<T, Derived> &) {}
    InitializeSEQ<T, Derived> &operator=
        (const InitializeSEQ<T, Derived> &) {return *this;}
    ~InitializeSEQ () {}
}; // class InitializeSEQ

/// \brief Sampler<T>::move_type subtype
/// \ingroup Sequential
template <typename T, typename Derived>
class MoveSEQ : public MoveBase<T, Derived>
{
    public :


    std::size_t operator() (std::size_t iter, Particle<T> &particle)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        std::size_t accept = 0;
        for (size_type i = 0; i != N; ++i)
            accept += this->move_state(iter, SingleParticle<T>(i, &particle));
        this->post_processor(iter, particle);

        return accept;
    }

    protected :

    MoveSEQ () {}
    MoveSEQ (const MoveSEQ<T, Derived> &) {}
    MoveSEQ<T, Derived> &operator=
        (const MoveSEQ<T, Derived> &) {return *this;}
    ~MoveSEQ () {}
}; // class MoveSEQ

/// \brief Monitor<T>::eval_type subtype
/// \ingroup Sequential
template <typename T, typename Derived>
class MonitorEvalSEQ : public MonitorEvalBase<T, Derived>
{
    public :


    void operator() (std::size_t iter, std::size_t dim,
            const Particle<T> &particle, double *res)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        for (size_type i = 0; i != N; ++i) {
            this->monitor_state(iter, dim,
                    ConstSingleParticle<T>(i, &particle), res + i * dim);
        }
        this->post_processor(iter, particle);
    }

    protected :

    MonitorEvalSEQ () {}
    MonitorEvalSEQ (const MonitorEvalSEQ<T, Derived> &) {}
    MonitorEvalSEQ<T, Derived> &operator=
        (const MonitorEvalSEQ<T, Derived> &) {return *this;}
    ~MonitorEvalSEQ () {}
}; // class MonitorEvalSEQ

/// \brief Path<T>::eval_type subtype
/// \ingroup Sequential
template <typename T, typename Derived>
class PathEvalSEQ : public PathEvalBase<T, Derived>
{
    public :


    double operator() (std::size_t iter, const Particle<T> &particle,
            double *res)
    {
        typedef typename Particle<T>::size_type size_type;
        const size_type N = static_cast<size_type>(particle.value().size());
        this->pre_processor(iter, particle);
        for (size_type i = 0; i != N; ++i) {
            res[i] = this->path_state(iter,
                    ConstSingleParticle<T>(i, &particle));
        }
        this->post_processor(iter, particle);

        return this->path_grid(iter, particle);
    }

    protected :

    PathEvalSEQ () {}
    PathEvalSEQ (const PathEvalSEQ<T, Derived> &) {}
    PathEvalSEQ<T, Derived> &operator=
        (const PathEvalSEQ<T, Derived> &) {return *this;}
    ~PathEvalSEQ () {}
}; // class PathEvalSEQ

} // namespace vsmc

#endif // VSMC_SMP_BACKEND_SEQ_HPP