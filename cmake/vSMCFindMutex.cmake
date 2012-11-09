SET (VSMC_MUTEX_TEST_SOURCE "
#include <vsmc/cxx11/mutex.hpp>

vsmc::cxx11::mutex mtx;
int mtx_d;

void op1 ()
{
    vsmc::cxx11::lock_guard<vsmc::cxx11::mutex> guard(mtx);
    mtx_d = 1;
}

void op2 ()
{
    vsmc::cxx11::lock_guard<vsmc::cxx11::mutex> guard(mtx);
    mtx_d = 2;
}

int main ()
{
    op1();
    op2();

    return 0;
}
")

IF (NOT VSMC_MUTEX_FOUND)
    UNSET (VSMC_MUTEX_FOUND       CACHE)
    UNSET (VSMC_MUTEX_STD_FOUND   CACHE)
    UNSET (VSMC_MUTEX_BOOST_FOUND CACHE)
    IF (NOT Boost_FOUND)
        INCLUDE (FindBoost)
    ENDIF (NOT Boost_FOUND)
    INCLUDE (FindThreads)
    INCLUDE (CheckCXXSourceRuns)
    FIND_PACKAGE (Boost OPTIONAL_COMPONENTS chrono date_time thread system)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    SET (SAFE_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRAREIS})
    SET (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES}
        ${Boost_INCLUDE_DIRS})
    SET (CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES}
        ${CMAKE_MUTEX_LIBS_INIT})
    SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_MUTEX=1)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_MUTEX_TEST_SOURCE}"
        VSMC_MUTEX_STD_FOUND)
    IF (Boost_FOUND)
        SET (CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES}
            ${Boost_CHRONO_LIBRARY_DEBUG} ${Boost_DATE_TIME_LIBRARY_DEBUG}
            ${Boost_THREAD_LIBRARY_DEBUG} ${Boost_SYSTEM_LIBRARY_DEBUG}
            ${CMAKE_THREAD_LIBS_INIT})
        SET (CMAKE_REQUIRED_DEFINITIONS -DVSMC_HAS_CXX11LIB_MUTEX=0)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_MUTEX_TEST_SOURCE}"
            VSMC_MUTEX_BOOST_FOUND)
    ENDIF (Boost_FOUND)
    IF (VSMC_MUTEX_STD_FOUND OR VSMC_MUTEX_BOOST_FOUND)
        SET (VSMC_MUTEX_FOUND TRUE CACHE BOOL "Found mutex")
    ENDIF (VSMC_MUTEX_STD_FOUND OR VSMC_MUTEX_BOOST_FOUND)
    IF (VSMC_MUTEX_STD_FOUND)
        SET (VSMC_MUTEX_STD_LINK_LIBRARIES ${CMAKE_MUTEX_LIBS_INIT}
            CACHE STRING "Link to STD mutex")
    ENDIF (VSMC_MUTEX_STD_FOUND)
    IF (VSMC_MUTEX_BOOST_FOUND)
        SET (VSMC_MUTEX_BOOST_LINK_LIBRARIES
            ${Boost_CHRONO_LIBRARY} ${Boost_DATE_TIME_LIBRARY}
            ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY}
            ${CMAKE_THREAD_LIBS_INIT}
            CACHE STRING "LINK to Boost mutex")
    ENDIF (VSMC_MUTEX_BOOST_FOUND)
    IF (VSMC_MUTEX_STD_FOUND)
        SET (VSMC_MUTEX_LINK_LIBRARIES ${VSMC_MUTEX_STD_LINK_LIBRARIES}
            CACHE STRING "Link to mutex")
    ELSEIF (VSMC_MUTEX_BOOST_FOUND)
        SET (VSMC_MUTEX_LINK_LIBRARIES ${VSMC_MUTEX_BOOST_LINK_LIBRARIES}
            CACHE STRING "Link to mutex")
    ENDIF (VSMC_MUTEX_STD_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRAREIS})
ENDIF (NOT VSMC_MUTEX_FOUND)