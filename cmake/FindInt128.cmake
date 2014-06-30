# ============================================================================
#  cmake/FindInt128.cmake
# ----------------------------------------------------------------------------
#
#                          vSMC: Scalable Monte Carlo
#
#  This file is distribured under the 2-clauses BSD License.
#  See LICENSE for details.
# ============================================================================

# Find 128-bits integer type
#
# The following variable is set
#
# INT128_FOUND - TRUE if 128-bits integer type is found and works correctly
# INT128_TYPE  - The type of the 128-bits integer

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/FindInt128.cpp INT128_TEST_SOURCE)

IF (NOT DEFINED INT128_FOUND)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})

    SET (INT128_TRY_TYPE "__int128")
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
        -DINT128=${INT128_TRY_TYPE})
    CHECK_CXX_SOURCE_RUNS ("${INT128_TEST_SOURCE}" INT128_FOUND)

    IF (INT128_FOUND)
        SET (INT128_TYPE ${INT128_TRY_TYPE} CACHE STRING "128-bits type")
        MESSAGE (STATUS "Found 128-bits integer type: ${INT128_TYPE}")
    ELSE (INT128_FOUND)
        MESSAGE (STATUS "NOT Found 128-bits integer type")
    ENDIF (INT128_FOUND)

    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
ENDIF (NOT DEFINED INT128_FOUND)
