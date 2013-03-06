IF (NOT VSMC_MPI_FOUND)
    UNSET (VSMC_MPI_FOUND CACHE)
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
        IF (NOT VSMC_MPIEXEC)
            SET (VSMC_MPIEXEC ${MPIEXEC} CACHE STRING "MPIEXEC")
        ENDIF (NOT VSMC_MPIEXEC)
        IF (NOT VSMC_MPIEXEC_NUMPROC_FLAG)
            SET (VSMC_MPIEXEC_NUMPROC_FLAG ${MPIEXEC_NUMPROC_FLAG}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT VSMC_MPIEXEC_NUMPROC_FLAG)
        IF (NOT VSMC_MPIEXEC_PREFLAGS)
            SET (VSMC_MPIEXEC_PREFLAGS ${MPIEXEC_PREFLAGS}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT VSMC_MPIEXEC_PREFLAGS)
        IF (NOT VSMC_MPIEXEC_POSTFLAGS)
            SET (VSMC_MPIEXEC_POSTFLAGS ${MPIEXEC_POSTFLAGS}
                CACHE STRING "MPIEXEC")
        ENDIF (NOT VSMC_MPIEXEC_POSTFLAGS)
    ELSE (MPI_CXX_FOUND AND Boost_FOUND)
        SET (VSMC_MPI_FOUND FALSE CACHE BOOL "NOT Found MPI")
    ENDIF (MPI_CXX_FOUND AND Boost_FOUND)
ENDIF (NOT VSMC_MPI_FOUND)
