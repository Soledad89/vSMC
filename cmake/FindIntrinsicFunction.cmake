# ============================================================================
#  cmake/FindIntrinsicFunction.cmake
# ----------------------------------------------------------------------------
#
#                          vSMC: Scalable Monte Carlo
#
#  This file is distribured under the 2-clauses BSD License.
#  See LICENSE for details.
# ============================================================================

# Find intrinsic function support
#
# The following variable is set
#
# INTRINSIC_FUNCTION_FOUND        - TRUE if intrinsic function
# INTRINSIC_INT64_FOUND           - TRUE if __int64 is defined
# INTRINSIC_INT64_LONG_LONG_FOUND - TRUE if __int64 is long long
#                                   FALSE if it is long

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/FindIntrinsicFunction.cpp
    INTRINSIC_FUNCTION_TEST_SOURCE)

IF (NOT DEFINED INTRINSIC_FUNCTION_FOUND)
    INCLUDE (CheckCXXSourceRuns)
    IF (${CMAKE_CXX_COMPILER_ID} MATCHES "Intel" OR MSVC)
        CHECK_CXX_SOURCE_RUNS ("${INTRINSIC_FUNCTION_TEST_SOURCE}"
            INTRINSIC_INT64_FOUND)
    ENDIF (${CMAKE_CXX_COMPILER_ID} MATCHES "Intel" OR MSVC)
    IF (NOT INTRINSIC_INT64_FOUND)
        CHECK_CXX_SOURCE_RUNS ("${INTRINSIC_FUNCTION_TEST_SOURCE}"
            INTRINSIC_INT64_LONG_LONG_FOUND)
    ENDIF (NOT INTRINSIC_INT64_FOUND)
    IF (NOT INTRINSIC_INT64_FOUND AND NOT INTRINSIC_INT64_LONG_LONG_FOUND)
        CHECK_CXX_SOURCE_RUNS ("${INTRINSIC_FUNCTION_TEST_SOURCE}"
            INTRINSIC_INT64_LONG_FOUND)
    ENDIF (NOT INTRINSIC_INT64_FOUND AND NOT INTRINSIC_INT64_LONG_LONG_FOUND)

    IF (INTRINSIC_INT64_FOUND OR
            INTRINSIC_INT64_LONG_LONG_FOUND OR INTRINSIC_INT64_LONG_FOUND)
        MESSAGE (STATUS "Found intrinsic function")
        SET (INTRINSIC_FUNCTION_FOUND TRUE CACHE BOOL
            "Found intrinsic function")
    ELSE (INTRINSIC_INT64_FOUND OR
            INTRINSIC_INT64_LONG_LONG_FOUND OR INTRINSIC_INT64_LONG_FOUND)
        MESSAGE (STATUS "NOT Found intrinsic function")
        SET (INTRINSIC_FUNCTION_FOUND FALSE CACHE BOOL
            "NOT Found intrinsic function")
    ENDIF (INTRINSIC_INT64_FOUND OR
        INTRINSIC_INT64_LONG_LONG_FOUND OR INTRINSIC_INT64_LONG_FOUND)
ENDIF (NOT DEFINED INTRINSIC_FUNCTION_FOUND)
