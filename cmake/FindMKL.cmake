# ============================================================================
#  vSMC/cmake/FindMKL.cmake
# ----------------------------------------------------------------------------
#                          vSMC: Scalable Monte Carlo
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2015, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

# Find Intel Math Kernel Library
#
# This module can be used to find MKL headers and libraries
#
# The following variables are set
#
# MKL_FOUND          - TRUE if MKL is found and work correctly
# MKL_INCLUDE_DIR    - The directory containing MKL headers, e.g., mkl_vml.h
# MKL_LINK_LIBRARIES - MKL libraries that shall be linked to
#
# The following variables affect the behavior of this module
#
# MKL_INC_PATH - The path CMake shall try to find headers first
# MKL_LIB_PATH - The path CMake shall try to find libraries first

IF (DEFINED MKL_FOUND)
    RETURN ()
ENDIF (DEFINED MKL_FOUND)

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/FindMKL.cpp MKL_TEST_SOURCE)

IF (NOT DEFINED MKL_LINK_LIBRARIES)
    INCLUDE (FindThreads)
    FIND_LIBRARY (MKL_LINK_LIBRARIES mkl_rt
        PATHS ${MKL_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    FIND_LIBRARY (MKL_LINK_LIBRARIES mkl_rt)
    FIND_LIBRARY (MKL_LINK_LIBRARIES mkl_rt_dll
        PATHS ${MKL_LIB_PATH} ENV LIB NO_DEFAULT_PATH)
    FIND_LIBRARY (MKL_LINK_LIBRARIES mkl_rt_dll)
    IF (MKL_LINK_LIBRARIES)
        MESSAGE (STATUS "Found MKL libraries: ${MKL_LINK_LIBRARIES}")
        SET (MKL_LINK_LIBRARIES ${MKL_LINK_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "MKL Libraries" )
    ELSE (MKL_LINK_LIBRARIES)
        MESSAGE (STATUS "NOT Found MKL libraries")
    ENDIF (MKL_LINK_LIBRARIES)
ENDIF (NOT DEFINED MKL_LINK_LIBRARIES)

IF (NOT DEFINED MKL_INCLUDE_DIR)
    FIND_PATH (MKL_INCLUDE_DIR mkl_vml.h
        PATHS ${MKL_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    FIND_PATH (MKL_INCLUDE_DIR mkl_vml.h)
    IF (MKL_INCLUDE_DIR)
        MESSAGE (STATUS "Found MKL headers: ${MKL_INCLUDE_DIR}")
    ELSE (MKL_INCLUDE_DIR)
        MESSAGE (STATUS "NOT Found MKL headers")
    ENDIF (MKL_INCLUDE_DIR)
ENDIF (NOT DEFINED MKL_INCLUDE_DIR)

IF (MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)
    SET (MKL_BASIC_FOUND TRUE)
ELSE (MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)
    SET (MKL_BASIC_FOUND FALSE)
ENDIF (MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)

IF (MKL_BASIC_FOUND)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    SET (SAFE_CMAKE_REQUIRED_INCLUDES  ${CMAKE_REQUIRED_INCLUDES})
    SET (SAFE_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES}
        ${MKL_INCLUDE_DIR})
    IF (MKL_LINK_LIBRARIES_DEBUG)
        SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
            ${MKL_LINK_LIBRARIES_DEBUG})
    ELSE (MKL_LINK_LIBRARIES_DEBUG)
        SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
            ${MKL_LINK_LIBRARIES})
    ENDIF (MKL_LINK_LIBRARIES_DEBUG)
    CHECK_CXX_SOURCE_RUNS ("${MKL_TEST_SOURCE}" MKL_TEST_SOURCE_RUNS)
    IF (MKL_TEST_SOURCE_RUNS)
        MESSAGE (STATUS "Found MKL")
        SET (MKL_FOUND TRUE CACHE BOOL "Found MKL")
    ELSE (MKL_TEST_SOURCE_RUNS)
        MESSAGE (STATUS "NOT Found MKL")
        SET (MKL_FOUND FALSE CACHE BOOL "NOT Found MKL")
    ENDIF (MKL_TEST_SOURCE_RUNS)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES})
ELSE (MKL_BASIC_FOUND)
    MESSAGE (STATUS "NOT Found MKL")
    SET (MKL_FOUND FALSE CACHE BOOL "NOT Found MKL")
ENDIF (MKL_BASIC_FOUND)
