# ============================================================================
#  vSMC/cmake/FindCXX11LibChrono.cmake
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

# Find C++11 <chrono> support
#
# The following variable is set
#
# CXX11LIB_CHRONO_FOUND - TRUE if C++11 <chrono> is found and work correctly

IF (DEFINED CXX11LIB_CHRONO_FOUND)
    RETURN ()
ENDIF (DEFINED CXX11LIB_CHRONO_FOUND)

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/FindCXX11LibChrono.cpp
    CXX11LIB_CHRONO_TEST_SOURCE)

INCLUDE (CheckCXXSourceRuns)
CHECK_CXX_SOURCE_RUNS ("${CXX11LIB_CHRONO_TEST_SOURCE}" CXX11LIB_CHRONO_FOUND)
