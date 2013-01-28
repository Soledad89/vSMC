#ifndef VSMC_UTILITY_NUMERIC_BASE_HPP
#define VSMC_UTILITY_NUMERIC_BASE_HPP

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/assert.hpp>
#include <vsmc/internal/defines.hpp>
#include <vsmc/internal/forward.hpp>

#include <vsmc/cxx11/functional.hpp>
#include <vsmc/cxx11/type_traits.hpp>

#include <cstddef>
#include <vector>

namespace vsmc { namespace integrate {

/// \brief Numerical integration base dispatch class
/// \ingroup Integrate
template <typename Derived>
class NumericBase
{
    protected :

    typedef VSMC_SIZE_TYPE size_type;
    typedef cxx11::function<double (double)> eval_type;

    NumericBase () {}
    NumericBase (const NumericBase<Derived> &) {}
    NumericBase<Derived> &operator=
        (const NumericBase<Derived> &) {return *this;}
    VSMC_SMP_BASE_DESTRUCTOR_PREFIX ~NumericBase () {}

    /// \brief Integrate a segment on the grid
    ///
    /// \param a The leftmost of the segment
    /// \param b The rightmost of the segment
    /// \param eval The functor used for evaluation
    ///
    /// \note `eval` need a thread-safe copy if the implementation use
    /// parallelization, such as NumericTBB etc, in the sense that when
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
        VSMC_RUNTIME_ASSERT_DERIVED_BASE(NumericBase);
        return static_cast<Derived *>(this)->integrate_segment(a, b, eval);
    }

    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (*) (double, double, const eval_type &))
    {
        return Derived::integrate_segment(a, b, eval);
    }

    double integrate_segment_dispatch (double a, double b,
            const eval_type &eval,
            double (NumericBase::*) (double, double, const eval_type &))
    { VSMC_STATIC_ASSERT_NO_IMPL(integrate_segment); return 0;}
}; // class NumericBase

/// \brief Numerical integration base interface class
/// \ingroup Integrate
template <>
class NumericBase<VBase>
{
    public :

    typedef VSMC_SIZE_TYPE size_type;
    typedef cxx11::function<double (double)> eval_type;

    protected :

    NumericBase () {}
    NumericBase (const NumericBase<VBase> &) {}
    NumericBase<VBase> &operator=
        (const NumericBase<VBase> &) {return *this;}
    virtual ~NumericBase () {}

    virtual double integrate_segment (double, double, const eval_type &) = 0;
}; // class NumericBase<VBase>

} } // namespace vsmc::integrate

#endif // VSMC_UTILITY_NUMERIC_BASE_HPP
