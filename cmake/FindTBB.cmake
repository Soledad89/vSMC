# ============================================================================
#  cmake/FindTBB.cmake
# ----------------------------------------------------------------------------
#
#                          vSMC: Scalable Monte Carlo
#
#  This file is distribured under the 2-clauses BSD License.
#  See LICENSE for details.
# ============================================================================

# Find Intel Threading Building Blocks library
#
# This module can be used to find TBB headers and libraries
#
# The following variables are set
#
# TBB_FOUND          - TRUE if TBB is found and work correctly
# TBB_INCLUDE_DIR    - The directory containing TBB headers, e.g., tbb/tbb.h
# TBB_LINK_LIBRARIES - TBB libraries that shall be linked to
# TBB_DEFINITIONS    - TBB compile time definitions
#
# The following variables affect the behavior of this module
#
# TBB_INC_PATH - The path CMake shall try to find headers first
# TBB_LIB_PATH - The path CMake shall try to find libraries first

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/FindTBB.cpp TBB_TEST_SOURCE)

IF (NOT DEFINED TBB_LINK_LIBRARIES)
    INCLUDE (FindThreads)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_RELEASE_FOUND tbb
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_RELEASE_FOUND tbb)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_DEBUG_FOUND tbb_debug
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_DEBUG_FOUND tbb_debug)
    IF (TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
        SET (TBB_LINK_LIBRARIES
            optimized ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            debug ${TBB_LINK_LIBRARIES_DEBUG_FOUND} ${CMAKE_THREAD_LIBS_INIT}
            CACHE STRING "Link to TBB")
        SET (TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "Link to TBB Release")
        SET (TBB_LINK_LIBRARIES_DEBUG ${TBB_LINK_LIBRARIES_DEBUG_FOUND}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "Link to TBB Debug")
        MESSAGE (STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
    ELSEIF (TBB_LINK_LIBRARIES_RELEASE_FOUND)
        SET (TBB_LINK_LIBRARIES ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "Link to TBB")
        SET (TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "Link to TBB Release")
        MESSAGE (STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
    ELSE (TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
        MESSAGE (STATUS "NOT Found TBB libraries")
    ENDIF (TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
ENDIF (NOT DEFINED TBB_LINK_LIBRARIES)

IF (NOT DEFINED TBB_INCLUDE_DIR)
    FIND_PATH (TBB_INCLUDE_DIR tbb/tbb.h
        PATHS ${TBB_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    FIND_PATH (TBB_INCLUDE_DIR tbb/tbb.h)
    IF (TBB_INCLUDE_DIR)
        MESSAGE (STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
    ELSE (TBB_INCLUDE_DIR)
        MESSAGE (STATUS "NOT Found TBB headers")
    ENDIF (TBB_INCLUDE_DIR)
ENDIF (NOT DEFINED TBB_INCLUDE_DIR)

IF (TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)
    SET (TBB_BASIC_FOUND TRUE)
ELSE (TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)
    SET (TBB_BASIC_FOUND FALSE)
ENDIF (TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)

IF (NOT DEFINED TBB_FOUND)
    IF (TBB_BASIC_FOUND)
        INCLUDE (CheckCXXSourceRuns)
        SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
        SET (SAFE_CMAKE_REQUIRED_INCLUDES  ${CMAKE_REQUIRED_INCLUDES})
        SET (SAFE_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
        SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES}
            ${TBB_INCLUDE_DIR})
        IF (TBB_LINK_LIBRARIES_DEBUG)
            SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
                ${TBB_LINK_LIBRARIES_DEBUG})
        ELSE (TBB_LINK_LIBRARIES_DEBUG)
            SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
                ${TBB_LINK_LIBRARIES})
        ENDIF (TBB_LINK_LIBRARIES_DEBUG)
        CHECK_CXX_SOURCE_RUNS ("${TBB_TEST_SOURCE}" TBB_TEST_SOURCE_RUNS)
        FOREACH (EXCEPTION_FLAG 1 0)
            FOREACH (CPP0X_FLAG 1 0)
                IF (NOT TBB_TEST_SOURCE_RUNS)
                    MESSAGE (STATUS
                        "Try TBB with -DTBB_USE_CAPTURED_EXCEPTION=${EXCEPTION_FLAG} -DTBB_IMPLEMENT_CPP0X=${CPP0X_FLAG}")
                    UNSET (TBB_TEST_SOURCE_RUNS CACHE)
                    SET (CMAKE_REQUIRED_DEFINITIONS
                        ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
                        -DTBB_USE_CAPTURED_EXCEPTION=${EXCEPTION_FLAG}
                        -DTBB_IMPLEMENT_CPP0X=${CPP0X_FLAG})
                    CHECK_CXX_SOURCE_RUNS ("${TBB_TEST_SOURCE}"
                        TBB_TEST_SOURCE_RUNS)
                    IF (TBB_TEST_SOURCE_RUNS)
                        SET (TBB_DEFINITIONS
                            -DTBB_USE_CAPTURED_EXCEPTION=${EXCEPTION_FLAG}
                            -DTBB_IMPLEMENT_CPP0X=${CPP0X_FLAG}
                            CACHE STRING "TBB compile time definitions")
                    ENDIF (TBB_TEST_SOURCE_RUNS)
                ENDIF (NOT TBB_TEST_SOURCE_RUNS)
            ENDFOREACH (CPP0X_FLAG 0 1)
        ENDFOREACH (EXCEPTION_FLAG 0 1)
        IF (TBB_TEST_SOURCE_RUNS)
            MESSAGE (STATUS "Found TBB")
            SET (TBB_FOUND TRUE CACHE BOOL "Found TBB")
        ELSE (TBB_TEST_SOURCE_RUNS)
            MESSAGE (STATUS "NOT Found TBB")
            SET (TBB_FOUND FALSE CACHE BOOL "NOT Found TBB")
        ENDIF (TBB_TEST_SOURCE_RUNS)
        SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
        SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
        SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES})
    ELSE (TBB_BASIC_FOUND)
        MESSAGE (STATUS "NOT Found TBB")
        SET (TBB_FOUND FALSE CACHE BOOL "NOT Found TBB")
    ENDIF (TBB_BASIC_FOUND)
ENDIF (NOT DEFINED TBB_FOUND)
