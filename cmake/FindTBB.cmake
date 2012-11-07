SET (TBB_FOUND TRUE)

INCLUDE (FindThreads)

IF (NOT TBB_LINK_LIBRARIES)
    IF (MSVC)
        IF (NOT TBB_LIB_PATH)
            IF (CMAKE_CL_64)
                SET (TBB_MSVC_LIB "intel64")
            ELSE (CMAKE_CL_64)
                SET (TBB_MSVC_LIB "ia32")
            ENDIF (CMAKE_CL_64)
            IF (MSVC_VERSION GREATER 1599)
                SET (TBB_MSVC_LIB "${TBB_MSVC_LIB}/vc10")
            ELSEIF (MSVC_VERSION GREATER 1499)
                SET (TBB_MSVC_LIB "${TBB_MSVC_LIB}/vc9")
            ELSEIF (MSVC_VERSION GREATER 1399)
                SET (TBB_MSVC_LIB "${TBB_MSVC_LIB}/vc8")
            ENDIF (MSVC_VERSION GREATER 1599)
            SET (TBB_LIB_PATH "${TBB_ROOT}/lib/${TBB_MSVC_LIB}")
            SET (TBB_DLL_PATH "${TBB_ROOT}/bin/${TBB_MSVC_LIB}")
        ENDIF (NOT TBB_LIB_PATH)
    ENDIF (MSVC)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/intel64)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/ia32)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/intel64)
    SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/ia32)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_RELEASE tbb
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH)
    FIND_LIBRARY (TBB_LINK_LIBRARIES_DEBUG tbb_debug
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH)
ENDIF (NOT TBB_LINK_LIBRARIES)

IF (TBB_LINK_LIBRARIES_RELEASE AND TBB_LINK_LIBRARIES_DEBUG)
    SET (TBB_LINK_LIBRARIES
        optimized ${TBB_LINK_LIBRARIES_RELEASE}
        debug ${TBB_LINK_LIBRARIES_DEBUG} ${CMAKE_THREAD_LIBS_INIT})
    SET (TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE}
        ${CMAKE_THREAD_LIBS_INIT})
    SET (TBB_LINK_LIBRARIES_DEBUG ${TBB_LINK_LIBRARIES_DEBUG}
        ${CMAKE_THREAD_LIBS_INIT})
    MESSAGE (STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
ELSEIF (TBB_LINK_LIBRARIES_RELEASE)
    SET (TBB_LINK_LIBRARIES ${TBB_LINK_LIBRARIES_RELEASE}
        ${CMAKE_THREAD_LIBS_INIT})
    SET (TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE}
        ${CMAKE_THREAD_LIBS_INIT})
    MESSAGE (STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
ELSE (TBB_LINK_LIBRARIES_RELEASE AND TBB_LINK_LIBRARIES_DEBUG)
    MESSAGE (STATUS "NOT Found TBB libraries")
    SET (TBB_FOUND FALSE)
ENDIF (TBB_LINK_LIBRARIES_RELEASE AND TBB_LINK_LIBRARIES_DEBUG)

IF (NOT TBB_INCLUDE_DIR)
    IF (MSVC)
        IF (NOT TBB_INC_PATH)
            SET (TBB_INC_PATH "${TBB_ROOT}/include")
        ENDIF (NOT TBB_INC_PATH)
    ENDIF(MSVC)
    SET (TBB_INC_PATH ${TBB_INC_PATH} $ENV{TBBROOT}/include)
    FIND_PATH (TBB_INCLUDE_DIR tbb/tbb.h PATHS ${TBB_INC_PATH} ENV CPATH)
ENDIF (NOT TBB_INCLUDE_DIR)

IF (TBB_INCLUDE_DIR)
    MESSAGE (STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
ELSE (TBB_INCLUDE_DIR)
    MESSAGE (STATUS "NOT Found TBB headers")
    SET (TBB_FOUND FALSE)
ENDIF (TBB_INCLUDE_DIR)

IF (WIN32)
    SET (TBB_DLL_PATH ${TBB_DLL_PATH} ${TBB_ROOT}/bin)
    IF (NOT TBB_DLL_RELEASE)
        FIND_FILE (TBB_DLL_RELEASE tbb.dll PATHS ${TBB_DLL_PATH}
            $ENV{TBBROOT}/bin)
    ENDIF (NOT TBB_DLL_RELEASE)
    IF (NOT TBB_DLL_DEBUG)
        FIND_FILE (TBB_DLL_DEBUG tbb_debug.dll PATHS ${TBB_DLL_PATH}
            $ENV{TBBROOT}/bin)
    ENDIF (NOT TBB_DLL_DEBUG)

    IF (TBB_DLL_RELEASE AND TBB_DLL_DEBUG)
        SET (TBB_DLL ${TBB_DLL_RELEASE} ${TBB_DLL_DEBUG})
        MESSAGE (STATUS "Found TBB DLL: ${TBB_DLL}")
    ELSE (TBB_DLL_RELEASE AND TBB_DLL_DEBUG)
        MESSAGE (STATUS "NOT Found TBB DLL")
        SET (TBB_FOUND FALSE)
    ENDIF (TBB_DLL_RELEASE AND TBB_DLL_DEBUG)
ENDIF (WIN32)


FUNCTION (ADD_TBB_RUNTIME exe_name)
    IF (MSVC)
        ADD_CUSTOM_COMMAND (
            OUTPUT ${PROJECT_BINARY_DIR}/tbb.dll
            DEPENDS ${TBB_DLL}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${TBB_DLL_RELEASE} ${PROJECT_BINARY_DIR}/tbb.dll)
        ADD_CUSTOM_TARGET (${exe_name}_tbb_dll
            DEPENDS ${PROJECT_BINARY_DIR}/tbb.dll)
        ADD_DEPENDENCIES (${exe_name} ${exe_name}_tbb_dll)
        ADD_CUSTOM_COMMAND (
            OUTPUT ${PROJECT_BINARY_DIR}/tbb_debug.dll
            DEPENDS ${TBB_DLL_DEBUG}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${TBB_DLL_DEBUG} ${PROJECT_BINARY_DIR}/tbb_debug.dll)
        ADD_CUSTOM_TARGET (${exe_name}_tbb_debug_dll
            DEPENDS ${PROJECT_BINARY_DIR}/tbb_debug.dll)
        ADD_DEPENDENCIES (${exe_name} ${exe_name}_tbb_debug_dll)
    ENDIF (MSVC)

    IF (XCODE_VERSION)
        ADD_CUSTOM_COMMAND (
            OUTPUT ${PROJECT_BINARY_DIR}/libtbb.dylib
            DEPENDS ${TBB_LINK_LIBRARIES_RELEASE}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${TBB_LINK_LIBRARIES_RELEASE} ${PROJECT_BINARY_DIR}/libtbb.dylib)
        ADD_CUSTOM_TARGET (${exe_name}_tbb_dylib
            DEPENDS ${PROJECT_BINARY_DIR}/libtbb.dylib)
        ADD_DEPENDENCIES (${exe_name} ${exe_name}_tbb_dylib)
    ENDIF (XCODE_VERSION)
ENDFUNCTION (ADD_TBB_RUNTIME)
