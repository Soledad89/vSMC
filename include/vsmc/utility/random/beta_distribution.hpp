#ifndef VSMC_UTILITY_RANDOM_BETA_DISTRIBUTION_HPP
#define VSMC_UTILITY_RANDOM_BETA_DISTRIBUTION_HPP

#include <vsmc/utility/random/common.hpp>

namespace vsmc { namespace cxx11 {

/// \brief Beta distribution
/// \ingroup Random
template <typename RealType = double>
class beta_distribution
{
    public :

    typedef RealType result_type;

    class param_type
    {
        public :

        typedef RealType result_type;
        typedef beta_distribution<RealType> distribution_type;

        param_type (RealType shape1 = 1, RealType shape2 = 1) :
            shape1_(shape1), shape2_(shape2)
        {
            VSMC_RUNTIME_ASSERT((shape1 > 0),
                    ("**vsmc::cxx11::beta_distribution** INVALID SHAPE1"));
            VSMC_RUNTIME_ASSERT((shape2 > 0),
                    ("**vsmc::cxx11::beta_distribution** INVALID SHAPE2"));
        }

        RealType shape1 () const {return shape1_;}

        RealType shape2 () const {return shape2_;}

        private :

        RealType shape1_;
        RealType shape2_;
    }; // param_type

    beta_distribution (RealType shape1 = 1, RealType shape2 = 1) :
        param_(param_type(shape1, shape2)),
        rgamma1_(shape1, 1), rgamma2_(shape2, 1) {}

    beta_distribution (const param_type &par) :
        param_(par), rgamma1_(param_.shape1()), rgamma2_(param_.shape2()) {}

    template <typename Eng>
    RealType operator() (Eng &eng)
    {
        RealType x = rgamma1_(eng);
        RealType y = rgamma2_(eng);

        return x / (x + y);
    }

    void reset () {};

    RealType shape1 () const {return param_.shape1();}

    RealType shape2 () const {return param_.shape2();}

    param_type param () const {return param_;}

    void param (const param_type &par) {param_ = par;}

    static VSMC_CONSTEXPR RealType min VSMC_MACRO_NO_EXPANSION () {return 0;}

    static VSMC_CONSTEXPR RealType max VSMC_MACRO_NO_EXPANSION () {return 1;}

    private :

    param_type param_;
    gamma_distribution<RealType> rgamma1_;
    gamma_distribution<RealType> rgamma2_;
}; // class beta_distribution

/// \brief Beta distribution comparison
/// \ingroup Random
template <typename RealType>
inline bool operator== (
        const typename beta_distribution<RealType>::param_type &p1,
        const typename beta_distribution<RealType>::param_type &p2)
{return p1.shape1() == p2.shape1() && p1.shape2() == p2.shape2();}

/// \brief Beta distribution comparison
/// \ingroup Random
template <typename RealType>
inline bool operator!= (
        const typename beta_distribution<RealType>::param_type &p1,
        const typename beta_distribution<RealType>::param_type &p2)
{return !(p1 == p2);}

/// \brief Beta distribution comparison
/// \ingroup Random
template <typename RealType>
inline bool operator== (
        const beta_distribution<RealType> &d1,
        const beta_distribution<RealType> &d2)
{return d1.shape1() == d2.shape1() && d1.shape2() == d2.shape2();}

/// \brief Beta distribution comparison
/// \ingroup Random
template <typename RealType>
inline bool operator!= (
        const beta_distribution<RealType> &d1,
        const beta_distribution<RealType> &d2)
{return !(d1 == d2);}

/// \brief Beta distribution output
/// \ingroup Random
template <typename CharT, typename Traits, typename RealType>
inline std::basic_ostream<CharT, Traits> &operator<< (
        std::basic_ostream<CharT, Traits> &os,
        const beta_distribution<RealType> &dist)
{
    internal::SaveIOFlags<CharT, Traits> flags(os);
    os.flags(std::ios_base::dec |
            std::ios_base::left |
            std::ios_base::fixed |
            std::ios_base::scientific);
    CharT sp = os.widen(' ');
    os.fill(sp);
    os << dist.shape1() << sp << dist.shape2();

    return os;
}

/// \brief Beta distribution input
/// \ingroup Random
template <typename CharT, typename Traits, typename RealType>
inline std::basic_istream<CharT, Traits> &operator>> (
        std::basic_istream<CharT, Traits> &is,
        beta_distribution<RealType> &dist)
{
    internal::SaveIOFlags<CharT, Traits> flags(is);
    is.flags(std::ios_base::dec | std::ios_base::skipws);
    RealType p1;
    RealType p2;
    is >> p1 >> p2;
    if (!is.fail())
        dist.param(typename beta_distribution<RealType>::param_type(p1, p2));

    return is;
}

} } // namespace vsmc::cxx11

#endif // VSMC_UTILITY_RANDOM_BETA_DISTRIBUTION_HPP