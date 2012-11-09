SET (VSMC_RANDOM_TEST_SOURCE "
#include <cassert>
#include <vsmc/cxx11/random.hpp>
#include <vsmc/core/rng.hpp>

int main ()
{
    const int N = 1000;
    vsmc::RngSetPrl eng(N);

    vsmc::cxx11::uniform_real_distribution<> runif(0, 1);
    for (int i = 0; i != N; ++i) {
        int u = runif(eng.rng(i));
        assert(u >= 0 && u <= 100);
    }

    vsmc::cxx11::uniform_int_distribution<> ruint(1, 100);
    for (int i = 0; i != N; ++i) {
        int u = ruint(eng.rng(i));
        assert(u >= 1 && u <= 100);
    }

    vsmc::cxx11::bernoulli_distribution rbern(0.5);
    for (int i = 0; i != N; ++i){
        int b = rbern(eng.rng(i));
        assert(b == 0 || b == 1);
    }

    vsmc::cxx11::binomial_distribution<> rbinom(20, 0.7);
    for (int i = 0; i != N; ++i) {
        int b = rbinom(eng.rng(i));
        assert(b >= 0 && b <= 20);
    }

    vsmc::cxx11::gamma_distribution<> rgamma(1, 1);
    for (int i = 0; i != N; ++i) {
        double g = rgamma(eng.rng(i));
        assert(g >= 0);
    }

    vsmc::cxx11::lognormal_distribution<> rlnom(1, 1);
    for (int i = 0; i != N; ++i) {
        double l = rlnom(eng.rng(i));
        assert(l >= 0);
    }

    vsmc::cxx11::normal_distribution<> rnorm(0, 1);
    for (int i = 0; i != N; ++i) {
        double n = rnorm(eng.rng(i));
    }

    return 0;
}
")

IF (NOT VSMC_RANDOM_FOUND)
    UNSET (VSMC_RANDOM_FOUND       CACHE)
    UNSET (VSMC_RANDOM_STD_FOUND   CACHE)
    UNSET (VSMC_RANDOM_BOOST_FOUND CACHE)
    IF (NOT Boost_FOUND)
        INCLUDE (FindBoost)
    ENDIF (NOT Boost_FOUND)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES}
        ${Boost_INCLUDE_DIRS})
        SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_RANDOM=1)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
            VSMC_RANDOM_STD_FOUND)
    IF (Boost_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_RANDOM=0)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
        VSMC_RANDOM_BOOST_FOUND)
    ENDIF (Boost_FOUND)
    IF (VSMC_RANDOM_STD_FOUND OR VSMC_RANDOM_BOOST_FOUND)
        SET (VSMC_RANDOM_FOUND TRUE CACHE BOOL "Found random")
    ENDIF (VSMC_RANDOM_STD_FOUND OR VSMC_RANDOM_BOOST_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
ENDIF (NOT VSMC_RANDOM_FOUND)