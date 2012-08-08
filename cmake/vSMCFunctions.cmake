FUNCTION (ADD_EXAMPLE basename algs)
    FOREACH (exe ${EXAMPLE_EXECUTABLES})
        STRING (TOUPPER "-DUSE_${exe}" flags)

        IF (${exe} STREQUAL "tbb")
            ADD_TBB_RUNTIME(${basename})
        ENDIF (${exe} STREQUAL "tbb")

        FOREACH (alg ${algs})
            SET (exe_name ${basename}-${alg}-${exe})
            ADD_EXECUTABLE (${exe_name} ${basename}-${alg}.cpp)

            IF (${exe} STREQUAL "omp")
                SET_TARGET_PROPERTIES (${exe_name} PROPERTIES COMPILE_FLAGS
                    "${VSMC_TEST_FLAGS} ${flags} ${OpenMP_CXX_FLAGS}")
            ELSE (${exe} STREQUAL "omp")
                SET_TARGET_PROPERTIES (${exe_name} PROPERTIES COMPILE_FLAGS
                    "${VSMC_TEST_FLAGS} ${flags}")
            ENDIF (${exe} STREQUAL "omp")

            IF (${exe} STREQUAL "tbb")
                TARGET_LINK_LIBRARIES (${exe_name}
                    ${EXAMPLE_LINK_LIBRARIES} ${TBB_LINK_LIBRARIES})
            ELSEIF (${exe} STREQUAL "omp")
                TARGET_LINK_LIBRARIES (${exe_name}
                    ${EXAMPLE_LINK_LIBRARIES} ${OpenMP_LINK_LIBRARIES})
            ELSE (${exe} STREQUAL "tbb")
                TARGET_LINK_LIBRARIES (${exe_name} ${EXAMPLE_LINK_LIBRARIES})
            ENDIF (${exe} STREQUAL "tbb")

            ADD_DEPENDENCIES (${basename} ${exe_name})
        ENDFOREACH(alg)
    ENDFOREACH (exe)
ENDFUNCTION (ADD_EXAMPLE)

FUNCTION (COPY_FILE basename filename)
    ADD_CUSTOM_COMMAND (
        OUTPUT  ${PROJECT_BINARY_DIR}/${filename}
        DEPENDS ${PROJECT_SOURCE_DIR}/${filename}
        COMMAND ${CMAKE_COMMAND} ARGS -E copy
        ${PROJECT_SOURCE_DIR}/${filename} ${PROJECT_BINARY_DIR}/${filename})
    ADD_CUSTOM_TARGET (${basename}-${filename}
        DEPENDS ${PROJECT_BINARY_DIR}/${filename})
    ADD_DEPENDENCIES (${basename} ${basename}-${filename})
ENDFUNCTION (COPY_FILE)