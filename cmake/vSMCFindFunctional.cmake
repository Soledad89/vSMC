SET (VSMC_FUNCTIONAL_TEST_SOURCE "
#include <cassert>
#include <vsmc/cxx11/functional.hpp>

int fn (int a, double, double, double)
{
    return 2 * a;
}

void gn (int &a)
{
    a *= 2;
}

class cl
{
    public :

    typedef vsmc::cxx11::function<int (int, double, double, double)> f_type;

    cl (f_type f = VSMC_NULLPTR) : f_(f) {}

    int operator() (int a, double, double, double)
    {
        return f_(a, 0, 0, 0);
    }

    private :

    f_type f_;
};

int main ()
{
    vsmc::cxx11::function<int (int, double, double, double)> f;
    assert(!bool(f));

    f = fn;
    assert(bool(f));
    assert(f(2, 0, 0, 0) == 4);

    f = VSMC_NULLPTR;
    assert(!bool(f));

    cl c;
    c = cl(fn);
    assert(c(2, 0, 0, 0) == 4);

    int a = 1;
    gn(vsmc::cxx11::ref(a));
    assert(a == 2);

    return 0;
}
")

IF (NOT VSMC_FUNCTIONAL_FOUND)
    UNSET (VSMC_FUNCTIONAL_FOUND       CACHE)
    UNSET (VSMC_FUNCTIONAL_STD_FOUND   CACHE)
    UNSET (VSMC_FUNCTIONAL_BOOST_FOUND CACHE)
    IF (NOT Boost_FOUND)
        INCLUDE (FindBoost)
    ENDIF (NOT Boost_FOUND)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES}
        ${Boost_INCLUDE_DIRS})
    SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_FUNCTIONAL=1)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_FUNCTIONAL_TEST_SOURCE}"
        VSMC_FUNCTIONAL_STD_FOUND)
    IF (Boost_FOUND)
        SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_FUNCTIONAL=0)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_FUNCTIONAL_TEST_SOURCE}"
            VSMC_FUNCTIONAL_BOOST_FOUND)
    ENDIF (Boost_FOUND)
    IF (VSMC_FUNCTIONAL_STD_FOUND OR VSMC_FUNCTIONAL_BOOST_FOUND)
        SET (VSMC_FUNCTIONAL_FOUND TRUE CACHE BOOL "Found functional")
    ENDIF (VSMC_FUNCTIONAL_STD_FOUND OR VSMC_FUNCTIONAL_BOOST_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
ENDIF (NOT VSMC_FUNCTIONAL_FOUND)