#ifndef VSMC_RNG_THREEFRY_HPP
#define VSMC_RNG_THREEFRY_HPP

#include <vsmc/rng/common.hpp>

#define VSMC_STATIC_ASSERT_RNG_THREEFRY_RESULT_TYPE(ResultType) \
    VSMC_STATIC_ASSERT(                                                      \
            (cxx11::is_same<ResultType, uint32_t>::value ||                  \
             cxx11::is_same<ResultType, uint64_t>::value),                   \
            USE_ThreefryEngine_WITH_INTEGER_TYPE_OTHER_THAN_uint32_t_OR_uint64_t)

#define VSMC_STATIC_ASSERT_RNG_THREEFRY_SIZE(K) \
    VSMC_STATIC_ASSERT((K == 2 || K == 4),                                   \
            USE_ThreefryEngine_WITH_SIZE_OTHER_THAN_2_OR_4)

#define VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_0(R) \
    VSMC_STATIC_ASSERT((R > 0), USE_ThreefryEngine_WITH_ZERO_ROUND)

#define VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_2(K, R) \
    VSMC_STATIC_ASSERT((R <= 32 || K == 4),                                  \
            USE_ThreefryEngine_WITH_SIZE_2_AND_ROUNDS_LARGER_THAN_32)

#define VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_4(K, R) \
    VSMC_STATIC_ASSERT((R <= 72),                                            \
            USE_ThreefryEngine_WITH_SIZE_4_AND_ROUNDS_LARGER_THAN_72)

#define VSMC_STATIC_ASSERT_RNG_THREEFRY \
        VSMC_STATIC_ASSERT_RNG_THREEFRY_RESULT_TYPE(ResultType);             \
        VSMC_STATIC_ASSERT_RNG_THREEFRY_SIZE(K);                             \
        VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_0(R);                          \
        VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_2(K, R);                       \
        VSMC_STATIC_ASSERT_RNG_THREEFRY_ROUND_4(K, R);

#define VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(T, K, N, I, val) \
    template <> struct ThreefryRotateConstant < T, K, N, I > :               \
        public cxx11::integral_constant< unsigned, val > {};

namespace vsmc {

namespace internal {

template <typename> struct ThreefryPar;

template <> struct ThreefryPar<uint32_t> :
    public cxx11::integral_constant<uint32_t, 0x1BD11BDA> {};

template <> struct ThreefryPar<uint64_t> :
    public cxx11::integral_constant<uint64_t,
           (static_cast<uint64_t>(0xA9FC1A22) +
            (static_cast<uint64_t>(0x1BD11BDA) << 32))> {};

template <typename, std::size_t, std::size_t, std::size_t>
struct ThreefryRotateConstant;

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 0, 0, 13)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 1, 0, 15)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 2, 0, 26)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 3, 0,  6)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 4, 0, 17)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 5, 0, 29)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 6, 0, 16)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 2, 7, 0, 24)

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 0, 0, 10)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 1, 0, 11)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 2, 0, 13)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 3, 0, 23)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 4, 0,  6)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 5, 0, 17)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 6, 0, 25)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 7, 0, 18)

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 0, 1, 26)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 1, 1, 21)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 2, 1, 27)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 3, 1,  5)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 4, 1, 20)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 5, 1, 11)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 6, 1, 10)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint32_t, 4, 7, 1, 20)

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 0, 0, 16)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 1, 0, 42)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 2, 0, 12)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 3, 0, 31)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 4, 0, 16)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 5, 0, 32)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 6, 0, 24)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 2, 7, 0, 21)

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 0, 0, 14)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 1, 0, 52)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 2, 0, 23)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 3, 0,  5)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 4, 0, 25)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 5, 0, 46)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 6, 0, 58)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 7, 0, 32)

VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 0, 1, 16)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 1, 1, 57)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 2, 1, 40)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 3, 1, 37)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 4, 1, 33)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 5, 1, 12)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 6, 1, 22)
VSMC_DEFINE_RNG_THREEFRY_ROTATE_CONSTANT(uint64_t, 4, 7, 1, 32)

template <typename ResultType, unsigned N> struct ThreefryRotateImpl;

template <unsigned N>
struct ThreefryRotateImpl<uint32_t, N>
{
    static uint32_t rotate (uint32_t x) {return (x << left_ | x >> right_);}

    private :

    static VSMC_CONSTEXPR const unsigned left_ = N & 31;
    static VSMC_CONSTEXPR const unsigned right_ = (32 - N) & 31;
};

template <unsigned N>
struct ThreefryRotateImpl<uint64_t, N>
{
    static uint64_t rotate (uint64_t x) {return (x << left_ | x >> right_);}

    private :

    static VSMC_CONSTEXPR const unsigned left_ = N & 63;
    static VSMC_CONSTEXPR const unsigned right_ = (64 - N) & 63;
};

template <typename ResultType, std::size_t, std::size_t N, bool = (N > 0)>
struct ThreefryRotate {static void rotate (ResultType *) {}};

template <typename ResultType, std::size_t N>
struct ThreefryRotate<ResultType, 2, N, true>
{
    static void rotate (ResultType *state)
    {
        state[0] += state[1];
        state[1] = ThreefryRotateImpl<ResultType, ThreefryRotateConstant<
            ResultType, 2, r_, 0>::value>::rotate(state[1]);
        state[1] ^= state[0];
    }

    private :

    static VSMC_CONSTEXPR const unsigned r_ = (N - 1) % 8;
};

template <typename ResultType, std::size_t N>
struct ThreefryRotate<ResultType, 4, N, true>
{
    static void rotate (ResultType *state)
    {
        state[0] += state[i0_];
        state[i0_] = ThreefryRotateImpl<ResultType, ThreefryRotateConstant<
            ResultType, 4, r_, 0>::value>::rotate(state[i0_]);
        state[i0_] ^= state[0];

        state[2] += state[i2_];
        state[i2_] = ThreefryRotateImpl<ResultType, ThreefryRotateConstant<
            ResultType, 4, r_, 1>::value>::rotate(state[i2_]);
        state[i2_] ^= state[2];
    }

    private :

    static VSMC_CONSTEXPR const std::size_t i0_ = N % 2 ? 1 : 3;
    static VSMC_CONSTEXPR const std::size_t i2_ = N % 2 ? 3 : 1;
    static VSMC_CONSTEXPR const unsigned r_ = (N - 1) % 8;
};

template <typename ResultType, std::size_t, std::size_t N, bool = (N % 4 == 0)>
struct ThreefryInsert
{static void insert (ResultType *, const ResultType *) {}};

template <typename ResultType, std::size_t N>
struct ThreefryInsert<ResultType, 2, N, true>
{
    static void insert (ResultType *state, const ResultType *par)
    {
        state[0] += par[i0_];
        state[1] += par[i1_];
        state[1] += inc_;
    }

    private :

    static VSMC_CONSTEXPR const std::size_t inc_ = N / 4;
    static VSMC_CONSTEXPR const std::size_t i0_ = (inc_ + 0) % 3;
    static VSMC_CONSTEXPR const std::size_t i1_ = (inc_ + 1) % 3;
};

template <typename ResultType, std::size_t N>
struct ThreefryInsert<ResultType, 4, N, true>
{
    static void insert (ResultType *state, const ResultType *par)
    {
        state[0] += par[i0_];
        state[1] += par[i1_];
        state[2] += par[i2_];
        state[3] += par[i3_];
        state[3] += inc_;
    }

    private :

    static VSMC_CONSTEXPR const std::size_t inc_ = N / 4;
    static VSMC_CONSTEXPR const std::size_t i0_ = (inc_ + 0) % 5;
    static VSMC_CONSTEXPR const std::size_t i1_ = (inc_ + 1) % 5;
    static VSMC_CONSTEXPR const std::size_t i2_ = (inc_ + 2) % 5;
    static VSMC_CONSTEXPR const std::size_t i3_ = (inc_ + 3) % 5;
};

} // namespace vsmc::internal

/// \brief Threefry RNG engine reimplemented
/// \ingroup R123RNG
///
/// \details
/// This is a reimplementation of the algorithm Threefry as described in
/// [Parallel Random Numbers: As Easy as 1, 2, 3][r123paper] and implemented in
/// [Random123][r123lib].
///
/// [r123paper]:http://sc11.supercomputing.org/schedule/event_detail.php?evid=pap274
/// [r123lib]: https://www.deshawresearch.com/resources_random123.html
///
/// Depending on the compiler, it might be slightly faster than the original
/// implementation. I have observed a speedup at most of two-folds when using
/// Clang on a Haswell CPU. In some cases it is slightly slower (but more close
/// than the faster case).
///
/// The implementation is almost identical to the original. Compared to
/// `r123:Engine<Threefry2x32>` etc., when using the default constructor of the
/// one with a single seed, the output shall be exactly the same for the first
/// \f$2^n\f$ iterations, where \f$n\f$ is the number of bits (32 or 64).
/// Further iterations may produce different results, as vSMC increment the
/// counter slightly differently, but it still cover the same range and has the
/// same period as the original.
template <typename ResultType, std::size_t K, std::size_t R = 20>
class ThreefryEngine
{
    public :

    typedef ResultType result_type;
    typedef StaticVector<ResultType, K> ctr_type;
    typedef StaticVector<ResultType, K> key_type;

    explicit ThreefryEngine (result_type s = 0) : remain_(0)
    {
        VSMC_STATIC_ASSERT_RNG_THREEFRY;
        seed(s);
    }

    template <typename SeedSeq>
    explicit ThreefryEngine (SeedSeq &seq, typename cxx11::enable_if<
            !internal::is_seed_sequence<SeedSeq, ResultType>::value>::type * =
            VSMC_NULLPTR) : remain_(0)
    {
        VSMC_STATIC_ASSERT_RNG_THREEFRY;
        seed(seq);
    }

    ThreefryEngine (const ThreefryEngine<ResultType, K, R> &other) :
        key_(other.key_), ctr_(other.ctr_), res_(other.res_), par_(other.par_),
        remain_(other.remain_) {VSMC_STATIC_ASSERT_RNG_THREEFRY;}

    ThreefryEngine<ResultType, K, R> &operator= (
            const ThreefryEngine<ResultType, K, R> &other)
    {
        if (this != &other) {
            ctr_ = other.ctr_;
            res_ = other.res_;
            key_ = other.key_;
            par_ = other.par_;
            remain_ = other.remain_;
        }

        return *this;
    }

    void seed (result_type s)
    {
        ctr_.fill(0);
        res_.fill(0);
        key_.fill(0);
        key_[0] = s;
        init_par();
        remain_ = 0;
    }

    template <typename SeedSeq>
    void seed (SeedSeq &seq, typename cxx11::enable_if<
            !internal::is_seed_sequence<SeedSeq, ResultType>::value>::type * =
            VSMC_NULLPTR)
    {
        ctr_.fill(0);
        res_.fill(0);
        key_.fill(0);
        seq.generate(key_.begin(), key_.end());
        init_par();
        remain_ = 0;
    }

    const key_type &key () const {return key_;}

    const ctr_type &ctr () const {return ctr_;}

    void key (const key_type &k)
    {
        key_ = k;
        init_par();
        remain_ = 0;
    }

    void ctr (const ctr_type &c)
    {
        ctr_ = c;
        remain_ = 0;
    }

    result_type operator() ()
    {
        if (remain_ > 0)
            return res_[--remain_];

        internal::RngCounter<ResultType, K>::increment(ctr_.data());
        res_ = ctr_;
        generate<0>(res_.data(), par_.data(), cxx11::true_type());
        remain_ = K - 1;

        return res_[K - 1];
    }

    void discard (std::size_t nskip)
    {
        for (std::size_t i = 0; i != nskip; ++i)
            operator()();
    }

    static VSMC_CONSTEXPR const result_type _Min = 0;
    static VSMC_CONSTEXPR const result_type _Max = static_cast<result_type>(
            ~(static_cast<result_type>(0)));

    static VSMC_CONSTEXPR result_type min VSMC_MNE () {return _Min;}
    static VSMC_CONSTEXPR result_type max VSMC_MNE () {return _Max;}

    friend inline bool operator== (
            const ThreefryEngine<ResultType, K, R> &eng1,
            const ThreefryEngine<ResultType, K, R> &eng2)
    {
        return
            eng1.ctr_ == eng2.ctr_ &&
            eng1.key_ == eng2.key_ &&
            eng1.remain_ == eng2.remain_;
    }

    friend inline bool operator!= (
            const ThreefryEngine<ResultType, K, R> &eng1,
            const ThreefryEngine<ResultType, K, R> &eng2)
    {return !(eng1 == eng2);}

    template <typename CharT, typename Traits>
    friend inline std::basic_ostream<CharT, Traits> &operator<< (
            std::basic_ostream<CharT, Traits> &os,
            const ThreefryEngine<ResultType, K, R> &eng)
    {
        if (os) os << eng.ctr_ << ' ';
        if (os) os << eng.res_ << ' ';
        if (os) os << eng.key_ << ' ';
        if (os) os << eng.par_ << ' ';
        if (os) os << eng.remain_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend inline std::basic_istream<CharT, Traits> &operator>> (
            std::basic_istream<CharT, Traits> &is,
            ThreefryEngine<ResultType, K, R> &eng)
    {
        ThreefryEngine eng_tmp;
        if (is) is >> std::ws >> eng_tmp.ctr_;
        if (is) is >> std::ws >> eng_tmp.res_;
        if (is) is >> std::ws >> eng_tmp.key_;
        if (is) is >> std::ws >> eng_tmp.par_;
        if (is) is >> std::ws >> eng_tmp.remain_;
        if (is) eng = eng_tmp;

        return is;
    }

    private :

    ctr_type ctr_;
    ctr_type res_;
    key_type key_;
    StaticVector<ResultType, K + 1> par_;
    std::size_t remain_;

    void init_par ()
    {
        par_ = key_.template resize<K + 1>();
        par_[K] = internal::ThreefryPar<ResultType>::value;
        for (std::size_t i = 0; i != K; ++i)
            par_[K] ^= par_[i];
    }

    template <std::size_t>
    void generate (result_type *, result_type *, cxx11::false_type) {}

    template <std::size_t N>
    void generate (result_type *state, result_type *par, cxx11::true_type)
    {
        internal::ThreefryRotate<ResultType, K, N>::rotate(state);
        internal::ThreefryInsert<ResultType, K, N>::insert(state, par);
        generate<N + 1>(state, par, cxx11::integral_constant<bool, N < R>());
    }
}; // class ThreefryEngine

/// \brief Threefry RNG engine re-implemented
/// \ingroup R123RNG
typedef ThreefryEngine<uint32_t, 2> Threefry2x32;

/// \brief Threefry RNG engine re-implemented
/// \ingroup R123RNG
typedef ThreefryEngine<uint32_t, 4> Threefry4x32;

/// \brief Threefry RNG engine re-implemented
/// \ingroup R123RNG
typedef ThreefryEngine<uint64_t, 2> Threefry2x64;

/// \brief Threefry RNG engine re-implemented
/// \ingroup R123RNG
typedef ThreefryEngine<uint64_t, 4> Threefry4x64;

} // namespace vsmc

#endif // VSMC_RNG_THREEFRY_HPP
