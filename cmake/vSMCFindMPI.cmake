# ============================================================================
#  vSMC/cmake/vSMCFindMPI.cmake
# ----------------------------------------------------------------------------
#                          vSMC: Scalable Monte Carlo
# ----------------------------------------------------------------------------
#  Copyright (c) 2013,2014, Yan Zhou
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

IF (NOT DEFINED VSMC_MPI_FOUND)
    INCLUDE (FindMPI)
    IF (MPI_CXX_FOUND)
        MESSAGE (STATUS "Found MPI_CXX_INCLUDE_PATH: ${MPI_CXX_INCLUDE_PATH}")
    ENDIF (MPI_CXX_FOUND)
    IF (NOT Boost_MPI_FOUND OR NOT Boost_SERIALIZATION_FOUND)
        FIND_PACKAGE (Boost COMPONENTS mpi serialization)
    ENDIF (NOT Boost_MPI_FOUND OR NOT Boost_SERIALIZATION_FOUND)
    IF (MPI_CXX_FOUND AND Boost_FOUND)
        SET (VSMC_MPI_FOUND TRUE CACHE BOOL "Found MPI")
        SET (VSMC_MPI_LINK_LIBRARIES
            ${Boost_MPI_LIBRARY} ${Boost_SERIALIZATION_LIBRARY}
            ${MPI_CXX_LIBRARIES} CACHE STRING "MPI Link Libraries")
        IF (NOT DEFINED VSMC_MPIEXEC)
            SET (VSMC_MPIEXEC ${MPIEXEC} CACHE STRING "MPIEXEC")
        ENDIF (NOT DEFINED VSMC_MPIEXEC)
        IF (NOT DEFINED VSMC_MPIEXEC_NUMPROC_FLAG)
            SET (VSMC_MPIEXEC_NUMPROC_FLAG ${MPIEXEC_NUMPROC_FLAG}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT DEFINED VSMC_MPIEXEC_NUMPROC_FLAG)
        IF (NOT DEFINED VSMC_MPIEXEC_PREFLAGS)
            SET (VSMC_MPIEXEC_PREFLAGS ${MPIEXEC_PREFLAGS}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT DEFINED VSMC_MPIEXEC_PREFLAGS)
        IF (NOT DEFINED VSMC_MPIEXEC_POSTFLAGS)
            SET (VSMC_MPIEXEC_POSTFLAGS ${MPIEXEC_POSTFLAGS}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT DEFINED VSMC_MPIEXEC_POSTFLAGS)
        MESSAGE (STATUS "VSMC_MPI_LINK_LIBRARIES: ${VSMC_MPI_LINK_LIBRARIES}")
        MESSAGE (STATUS "VSMC_MPIEXEC: ${VSMC_MPIEXEC}")
        MESSAGE (STATUS "VSMC_MPIEXEC_NUMPROC_FLAG: ${VSMC_MPIEXEC_NUMPROC_FLAG}")
        MESSAGE (STATUS "VSMC_MPIEXEC_PREFLAGS: ${VSMC_MPIEXEC_PREFLAGS}")
        MESSAGE (STATUS "VSMC_MPIEXEC_POSTFLAGS: ${VSMC_MPIEXEC_POSTFLAGS}")
        MESSAGE (STATUS "Found MPI")
    ELSE (MPI_CXX_FOUND AND Boost_FOUND)
        MESSAGE (STATUS "NOT Found MPI")
        SET (VSMC_MPI_FOUND FALSE CACHE BOOL "NOT Found MPI")
    ENDIF (MPI_CXX_FOUND AND Boost_FOUND)
ENDIF (NOT DEFINED VSMC_MPI_FOUND)
