# ============================================================================
#  cmake/vSMCFindMPI.cmake
# ----------------------------------------------------------------------------
#
#                          vSMC: Scalable Monte Carlo
#
#  This file is distribured under the 2-clauses BSD License.
#  See LICENSE for details.
# ============================================================================

IF (NOT DEFINED VSMC_MPI_FOUND)
    INCLUDE (FindMPI)
    IF (MPI_CXX_FOUND)
        MESSAGE (STATUS "Found MPI_CXX_INCLUDE_PATH: ${MPI_CXX_INCLUDE_PATH}")
        MESSAGE (STATUS "Found MPIEXEC: ${MPIEXEC}")
    ENDIF (MPI_CXX_FOUND)
    IF (NOT Boost_MPI_FOUND OR NOT Boost_SERIALIZATION_FOUND)
        FIND_PACKAGE (Boost COMPONENTS mpi serialization)
    ENDIF (NOT Boost_MPI_FOUND OR NOT Boost_SERIALIZATION_FOUND)
    IF (MPI_CXX_FOUND AND Boost_FOUND)
        SET (VSMC_MPI_FOUND TRUE CACHE BOOL "Found MPI")
        SET (VSMC_MPI_LINK_LIBRARIES
            ${Boost_MPI_LIBRARY} ${Boost_SERIALIZATION_LIBRARY}
            ${MPI_CXX_LIBRARIES} CACHE STRING "MPI Link Libraries")
        MESSAGE (STATUS "VSMC_MPI_LINK_LIBRARIES: ${VSMC_MPI_LINK_LIBRARIES}")
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
    ELSE (MPI_CXX_FOUND AND Boost_FOUND)
        SET (VSMC_MPI_FOUND FALSE CACHE BOOL "NOT Found MPI")
    ENDIF (MPI_CXX_FOUND AND Boost_FOUND)
ENDIF (NOT DEFINED VSMC_MPI_FOUND)
