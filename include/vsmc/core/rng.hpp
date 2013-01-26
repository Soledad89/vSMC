#ifndef VSMC_CORE_RNG_HPP
#define VSMC_CORE_RNG_HPP

#include <vsmc/internal/common.hpp>

#if VSMC_USE_RANDOM123
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4521)
#endif // _MSC_VER
#define R123_USE_U01_DOUBLE 1
#include <Random123/philox.h>
#include <Random123/threefry.h>
#include <Random123/u01.h>
#include <Random123/MicroURNG.hpp>
#include <Random123/conventional/Engine.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
#endif // VSMC_USE_RANDOM123

namespace vsmc {

/// \brief Seed generator
/// \ingroup Core
class Seed
{
    public :

    typedef unsigned result_type;

    static Seed &instance ()
    {
        static Seed seed;

        return seed;
    }

    result_type get ()
    {
        if (seed_  >= std::numeric_limits<result_type>::max
                VSMC_MACRO_NO_EXPANSION () - 1)
            seed_ = 0;

        return ++seed_;
    }

    void set (result_type seed)
    {
        seed_ = seed;
    }

    void skip (result_type steps)
    {
        if (seed_ >= std::numeric_limits<result_type>::max
                VSMC_MACRO_NO_EXPANSION () - steps)
            seed_ = steps;
        else
            seed_ += steps;
    }

    private :

    result_type seed_;

    Seed () : seed_(VSMC_RNG_SEED)
    {
        if (!seed_)
            seed_ = std::rand();
    }

    Seed (const Seed &);
    Seed &operator= (const Seed &);
    ~Seed () {};
}; // class Seed

/// \brief Parallel RNG set class
/// \ingroup Core
class RngSet
{
    public :

    typedef VSMC_PRLRNG_TYPE rng_type;
    typedef VSMC_SEED_TYPE seed_type;
    typedef std::vector<rng_type>::size_type size_type;

    explicit RngSet (size_type N)
    {
        seed_type &seed = seed_type::instance();
        for (size_type i = 0; i != N; ++i)
            rng_.push_back(rng_type(static_cast<rng_type::result_type>(
                            seed.get())));
    }

    rng_type &rng (size_type id)
    {
        return rng_[id];
    }

    private :

    std::vector<rng_type> rng_;
}; // class RngSet

} // namespace vsmc

VSMC_DEFINE_TYPE_DISPATCH_TRAIT(RngSetType, rng_set_type, RngSet)
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(ResampleRngType, resample_rng_type,
        cxx11::mt19937)

#endif // VSMC_CORE_RNG_HPP
