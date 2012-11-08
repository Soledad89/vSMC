SET (CXX_MATH_ISFINITE_TEST_SOURCE "
#include <cassert>
#include <limits>
#include <cmath>

#ifdef CXX_MATH_ISFINITE_C99_FOUND
#define CXX_MATH_ISFINITE isfinite
#endif

#ifdef CXX_MATH_ISFINITE_STD_FOUND
#define CXX_MATH_ISFINITE std::isfinite
#endif

#ifdef CXX_MATH_ISFINITE_BOOST_FOUND
#include <boost/math/special_functions/fpclassify.hpp>
#define CXX_MATH_ISFINITE boost::math::isfinite
#endif

int main ()
{
    double x =  std::numeric_limits<double>::infinity();
    assert(!CXX_MATH_ISFINITE(x));

    return 0;
}
")

IF (NOT CXX_MATH_ISFINITE_FOUND)
    UNSET (CXX_MATH_ISFINITE_FOUND CACHE)
    UNSET (CXX_MATH_ISFINITE_C99_FOUND CACHE)
    UNSET (CXX_MATH_ISFINITE_STD_FOUND CACHE)
    UNSET (CXX_MATH_ISFINITE_BOOST_FOUND CACHE)
    INCLUDE (FindBoost)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES}
        ${Boost_INCLUDE_DIRS})
    CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ISFINITE_TEST_SOURCE}"
        CXX_MATH_ISFINITE_C99_FOUND)
    CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ISFINITE_TEST_SOURCE}"
        CXX_MATH_ISFINITE_STD_FOUND)
    IF (Boost_FOUND)
        CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ISFINITE_TEST_SOURCE}"
            CXX_MATH_ISFINITE_BOOST_FOUND)
    ENDIF (Boost_FOUND)
    IF (CXX_MATH_ISFINITE_C99_FOUND OR CXX_MATH_ISFINITE_STD_FOUND
            OR CXX_MATH_ISFINITE_BOOST_FOUND)
        SET (CXX_MATH_ISFINITE_FOUND TRUE CACHE BOOL "Found isfinite")
    ENDIF (CXX_MATH_ISFINITE_C99_FOUND OR CXX_MATH_ISFINITE_STD_FOUND
        OR CXX_MATH_ISFINITE_BOOST_FOUND)
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
ENDIF (NOT CXX_MATH_ISFINITE_FOUND)
