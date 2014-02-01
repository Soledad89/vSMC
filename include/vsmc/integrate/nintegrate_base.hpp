#ifndef VSMC_INTEGRATE_NINTEGRATE_BASE_HPP
#define VSMC_INTEGRATE_NINTEGRATE_BASE_HPP

#include <vsmc/internal/common.hpp>

#define VSMC_RUNTIME_ASSERT_INTEGRATE_NINTEGRATE_BASE_DERIVED \
    VSMC_RUNTIME_ASSERT((dynamic_cast<Derived *>(this)),                     \
            ("DERIVED FROM  NIntegrateBase                                   \
             " WITH INCORRECT **Derived** TEMPLATE PARAMTER"));

namespace vsmc {

/// \brief Numerical integration base dispatch class
/// \ingroup Integrate
template <typename Derived>
class NIntegrateBase
{
    protected :

    typedef std::size_t size_type;
    typedef cxx11::function<double (double)> eval_type;

    NIntegrateBase () {}
    NIntegrateBase (const NIntegrateBase<Derived> &) {}
    NIntegrateBase<Derived> &operator=
        (const NIntegrateBase<Derived> &) {return *this;}
    VSMC_CRTP_DESTRUCTOR_PREFIX ~NIntegrateBase () {}

    /// \brief Integrate a segment on the grid
    ///
    /// \param a The leftmost of the segment
    /// \param b The rightmost of the segment
    /// \param eval The functor used for evaluation
    ///
    /// `eval` need a thread-safe copy if the implementation use
    /// parallelization, such as NIntegrateTBB etc, in the sense that when
    /// constructing the new object, vSMC does not need to lock the original.
    /// Both major C++11 implementations and current Boost `function` satisfy
    /// this requirement, though the actual functor wrapped by
    /// `vsmc::cxx11::function` may not be. It is the user's responsibility to
    /// ensure this.
    double integrate_segment (double a, double b, const eval_type &eval)
    {
        return integrate_segment_dispatch(a, b, eval,
                &Derived::integrate_segment);
    }

    private :

    template <typename D>
    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (D::*) (double, double, const eval_type &))
    {
        VSMC_RUNTIME_ASSERT_INTEGRATE_NINTEGRATE_BASE_DERIVED;
        return static_cast<Derived *>(this)->integrate_segment(a, b, eval);
    }

    template <typename D>
    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (D::*) (double, double, const eval_type &) const)
    {
        VSMC_RUNTIME_ASSERT_INTEGRATE_NINTEGRATE_BASE_DERIVED;
        return static_cast<Derived *>(this)->integrate_segment(a, b, eval);
    }

    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (*) (double, double, const eval_type &))
    {return Derived::integrate_segment(a, b, eval);}

    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (NIntegrateBase::*) (double, double, const eval_type &))
    {VSMC_STATIC_ASSERT_NO_IMPL(integrate_segment); return 0;}
}; // class NIntegrateBase

/// \brief Numerical integration base dispatch class
/// \ingroup Integrate
template <>
class NIntegrateBase<Virtual>
{
    public :

    typedef std::size_t size_type;
    typedef cxx11::function<double (double)> eval_type;

    protected :

    NIntegrateBase () {}
    NIntegrateBase (const NIntegrateBase<Virtual> &) {}
    NIntegrateBase<Virtual> &operator=
        (const NIntegrateBase<Virtual> &) {return *this;}
    virtual ~NIntegrateBase () {}

    virtual double integrate_segment (double, double, const eval_type &) = 0;
}; // class NIntegrateBase<Virtual>

} // namespace vsmc

#endif // VSMC_INTEGRATE_NINTEGRATE_BASE_HPP