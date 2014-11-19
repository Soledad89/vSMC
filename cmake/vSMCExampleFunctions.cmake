# ============================================================================
#  vSMC/vSMCExample/cmake/vSMCExampleFunctions.cmake
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

FUNCTION (ADD_SMP_EXECUTABLE base header source smp_name)
    STRING (TOUPPER "${smp_name}" SMP)
    STRING (TOLOWER "${smp_name}" smp)

    IF (EXISTS ${PROJECT_SOURCE_DIR}/include/${header}.hpp)
        CONFIGURE_FILE (
            ${PROJECT_SOURCE_DIR}/include/${header}.hpp
            ${PROJECT_BINARY_DIR}/include/${header}_${smp}.hpp)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/include/${header}.hpp)
    CONFIGURE_FILE (
        ${PROJECT_SOURCE_DIR}/src/${source}.cpp
        ${PROJECT_BINARY_DIR}/src/${source}_${smp}.cpp)

    ADD_EXECUTABLE (${source}_${smp}
        ${PROJECT_BINARY_DIR}/src/${source}_${smp}.cpp)

    IF (${smp} STREQUAL "omp")
        SET_TARGET_PROPERTIES (${source}_${smp} PROPERTIES COMPILE_FLAGS
            "${OpenMP_CXX_FLAGS}")
        TARGET_LINK_LIBRARIES (${source}_${smp} ${OpenMP_LINK_LIBRARIES})
    ELSEIF (${smp} STREQUAL "std")
        TARGET_LINK_LIBRARIES (${source}_${smp} ${CMAKE_THREAD_LIBS_INIT})
    ELSEIF (${smp} STREQUAL "tbb")
        TARGET_LINK_LIBRARIES (${source}_${smp} ${TBB_LINK_LIBRARIES})
    ENDIF (${smp} STREQUAL "omp")

    IF (${source} MATCHES "_mpi")
        TARGET_LINK_LIBRARIES (${source}_${smp} ${VSMC_MPI_LINK_LIBRARIES})
        ADD_DEPENDENCIES (example_mpi ${source}_${smp})
    ENDIF (${source} MATCHES "_mpi")

    TARGET_LINK_LIBRARIES (${source}_${smp} ${VSMC_LINK_LIBRARIES})

    ADD_DEPENDENCIES (${base} ${source}_${smp})
    ADD_DEPENDENCIES (example_${smp} ${source}_${smp})
ENDFUNCTION (ADD_SMP_EXECUTABLE)

FUNCTION (ADD_EXAMPLE base)
    ADD_CUSTOM_TARGET (${base})
    ADD_CUSTOM_TARGET (${base}-files)
    ADD_DEPENDENCIES (${base} ${base}-files)
    ADD_DEPENDENCIES (example ${base})
    FOREACH (alg ${ARGN})
        ADD_CUSTOM_TARGET (${base}_${alg})
        FOREACH (smp ${SMP_EXECUTABLES})
            ADD_SMP_EXECUTABLE (${base} ${base} ${base}_${alg} ${smp})
            ADD_DEPENDENCIES (${base}_${alg} ${base}_${alg}_${smp})
        ENDFOREACH (smp)
    ENDFOREACH (alg)
ENDFUNCTION (ADD_EXAMPLE)

FUNCTION (COPY_FILE basename filename)
    IF (UNIX)
        ADD_CUSTOM_COMMAND (
            OUTPUT  ${PROJECT_BINARY_DIR}/${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E create_symlink
            ${PROJECT_SOURCE_DIR}/${filename}
            ${PROJECT_BINARY_DIR}/${filename})
    ELSE (UNIX)
        ADD_CUSTOM_COMMAND (
            OUTPUT  ${PROJECT_BINARY_DIR}/${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${PROJECT_SOURCE_DIR}/${filename}
            ${PROJECT_BINARY_DIR}/${filename})
    ENDIF (UNIX)
    ADD_CUSTOM_TARGET (${basename}-${filename}
        DEPENDS ${PROJECT_BINARY_DIR}/${filename})
    ADD_DEPENDENCIES (${basename}-files ${basename}-${filename})
ENDFUNCTION (COPY_FILE)

FUNCTION (COPY_FILE_OPTIONAL basename filename)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/${filename})
        COPY_FILE (${basename} ${filename})
    ELSE (EXISTS ${PROJECT_SOURCE_DIR}/${filename})
            MESSAGE (STATUS "File: ${basename}: ${filename} not available")
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/${filename})
ENDFUNCTION (COPY_FILE_OPTIONAL)

FUNCTION (VSMC_HEADER basepath cond)
    IF (${cond})
        STRING (REPLACE "/" "_" basename "${basepath}")
        IF (EXISTS ${PROJECT_SOURCE_DIR}/src/${basename}.cpp)
            ADD_EXECUTABLE (${basename}_hpp
                ${PROJECT_SOURCE_DIR}/src/${basename}.cpp)
        ELSE (EXISTS ${PROJECT_SOURCE_DIR}/src/${basename}.cpp)
            CONFIGURE_FILE (${PROJECT_SOURCE_DIR}/src/vsmc.cpp
                ${PROJECT_BINARY_DIR}/src/${basename}.cpp)
            ADD_EXECUTABLE (${basename}_hpp
                ${PROJECT_BINARY_DIR}/src/${basename}.cpp)
        ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/src/${basename}.cpp)

        FOREACH (arg ${ARGN})
            IF (${arg} STREQUAL "MPI")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${VSMC_MPI_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "MPI")
            IF (${arg} STREQUAL "OPENCL")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${OpenCL_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "OPENCL")
            IF (${arg} STREQUAL "GSL")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${GSL_LINK_LIBRARIES})
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${GSL_CBLAS_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "GSL")
            IF (${arg} STREQUAL "MKL")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${MKL_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "MKL")
            IF (${arg} STREQUAL "GCD")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${GCD_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "GCD")
            IF (${arg} STREQUAL "OPENMP")
                SET_TARGET_PROPERTIES (${basename}_hpp
                    PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${OpenMP_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "OPENMP")
            IF (${arg} STREQUAL "TBB")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${TBB_LINK_LIBRARIES})
            ENDIF (${arg} STREQUAL "TBB")
            IF (${arg} STREQUAL "THREAD")
                TARGET_LINK_LIBRARIES (${basename}_hpp
                    ${CMAKE_THREAD_LIBS_INIT})
            ENDIF (${arg} STREQUAL "THREAD")
        ENDFOREACH (arg ${ARGN})

        TARGET_LINK_LIBRARIES (${basename}_hpp ${VSMC_LINK_LIBRARIES})

        ADD_DEPENDENCIES (vsmc ${basename}_hpp)
    ENDIF (${cond})
ENDFUNCTION (VSMC_HEADER)
