#ifndef VSMC_RNG_COMBINE_HPP
#define VSMC_RNG_COMBINE_HPP

#include <vsmc/rng/common.hpp>

#define VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(ResultType) \
    VSMC_STATIC_ASSERT((::vsmc::cxx11::is_unsigned<ResultType>::value),      \
            USE_XorCombineEngine_WITH_A_ResultType_NOT_AN_UNSIGNED_INTEGER_TYPE)

#define VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2) \
    VSMC_STATIC_ASSERT((::vsmc::cxx11::is_same<                              \
                typename Eng1::resultType, typename Eng2::resultType>::value),\
            USE_XorCombineEngine_WITH_TWO_RNG_ENGINES_WITH_DIFFERENT_RESULT_TYPE)

namespace vsmc {

/// \brief Combine two RNG engines using XOR
/// \ingroup RNG
template <typename Eng1, typename Eng2,
         typename Eng1::result_type S1 = 0,
         typename Eng2::result_type S2 = 0>
class XorCombineEngine
{
    public :

    typedef typename Eng1::result_type result_type;
    typedef Eng1 engine_type1;
    typedef Eng2 engine_type2;

    explicit XorCombineEngine (result_type s = 123456) : eng1_(s), eng2_(s)
    {
        VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(result_type);
        VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2);
    }

    template <typename SeedSeq>
    explicit XorCombineEngine (SeedSeq &seq) : eng1_(seq), eng2_(seq)
    {
        VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(result_type);
        VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2);
    }

    XorCombineEngine (const XorCombineEngine<Eng1, Eng2> &other) :
        eng1_(other.eng1_), eng2_(other.eng2_)
    {
        VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(result_type);
        VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2);
    }

    XorCombineEngine (XorCombineEngine<Eng1, Eng2> &other) :
        eng1_(other.eng1_), eng2_(other.eng2_)
    {
        VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(result_type);
        VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2);
    }

    XorCombineEngine<Eng1, Eng2> &operator= (
            const XorCombineEngine<Eng1, Eng2> &other)
    {
        if (this != &other) {
            eng1_ = other.eng1_;
            eng2_ = other.eng2_;
        }

        return *this;
    }

#if VSMC_HAS_CXX11_RVALUE_REFERENCES
    XorCombineEngine (XorCombineEngine<Eng1, Eng2> &&other) :
        eng1_(cxx11::move(other.eng1_)), eng2_(cxx11::move(other.eng2_))
    {
        VSMC_STATIC_ASSERT_RNG_XOR_COMBINE_UNSIGNED(result_type);
        VSMC_STATIC_ASSERT_XOR_COMBINE_SAME_TYPE(Eng1, Eng2);
    }

    XorCombineEngine<Eng1, Eng2> &operator= (
            XorCombineEngine<Eng1, Eng2> &&other)
    {
        if (this != &other) {
            eng1_ = cxx11::move(other.eng1_);
            eng2_ = cxx11::move(other.eng2_);
        }

        return *this;
    }
#endif

    void seed (result_type s)
    {
        eng1_.seed(s);
        eng2_.seed(s);
    }

    template <typename SeedSeq>
    void seed (SeedSeq &seq)
    {
        eng1_.seed(seq);
        eng2_.seed(seq);
    }

    engine_type1 &eng1 () {return eng1_;}

    engine_type2 &eng2 () {return eng2_;}

    static VSMC_CONSTEXPR result_type min VSMC_MNE ()
    {
        return Eng1::min VSMC_MNE () < Eng2::min VSMC_MNE () ?
            Eng1::min VSMC_MNE : Eng2::min VSMC_MNE ();
    }

    static VSMC_CONSTEXPR result_type max VSMC_MNE ()
    {
        return Eng1::max VSMC_MNE () > Eng2::max VSMC_MNE () ?
            Eng1::max VSMC_MNE : Eng2::max VSMC_MNE ();
    }

    result_type operator() () {return (eng1_()<<S1)^(eng2_()<<S2);}

    void discard (std::size_t nskip)
    {
        for (std::size_t i = 0; i != nskip; ++i)
            operator()();
    }

    friend inline bool operator== (
            const XorCombineEngine<Eng1, Eng2> &eng1,
            const XorCombineEngine<Eng1, Eng2> &eng2)
    {return (eng1.eng1_ == eng2.eng1_ && eng1.eng2_ == eng2.eng2_);}

    friend inline bool operator!= (
            const XorCombineEngine<Eng1, Eng2> &eng1,
            const XorCombineEngine<Eng1, Eng2> &eng2)
    {return !(eng1 == eng2);}

    template <typename CharT, typename Traits>
    friend inline std::basic_ostream<CharT, Traits> &operator<< (
            std::basic_ostream<CharT, Traits> &os,
            const XorCombineEngine<Eng1, Eng2> &eng)
    {
        os << eng.eng1_ << ' ' << eng.eng2_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend inline std::basic_istream<CharT, Traits> &operator>> (
            std::basic_istream<CharT, Traits> &is,
            XorCombineEngine<Eng1, Eng2> &eng)
    {
        engine_type1 eng1;
        engine_type2 eng2;
        if (is >> eng1 >> std::ws >> eng2) {
            eng.eng1_ = eng1;
            eng.eng2_ = eng2;
        }

        return is;
    }

    private :

    engine_type1 eng1_;
    engine_type2 eng2_;
}; // class XorCombineEngine

} // namespace vsmc

#endif // VSMC_RNG_COMBINE_HPP