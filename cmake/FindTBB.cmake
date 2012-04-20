SET (TBB_FOUND TRUE)

SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/intel64)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/ia32)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/intel64)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/ia32)

FIND_LIBRARY (TBB_LINK_LIBRARIES tbb PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH)

IF (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)
    MESSAGE (STATUS "NOT Found TBB")
ELSE (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)
    MESSAGE (STATUS "Found TBB: ${TBB_LINK_LIBRARIES}")
ENDIF (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)

SET (TBB_INC_PATH ${TBB_INC_PATH} $ENV{TBBROOT}/include)
FIND_PATH (TBB_INCLUDE_DIR tbb/tbb.h PATHS ${TBB_INC_PATH} ENV CPATH)
IF (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)
    MESSAGE (STATUS "NOT Found TBB headers")
    SET (TBB_FOUND FALSE)
ELSE (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)
    MESSAGE (STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
ENDIF (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)

IF (MSVC)
    SET (TBB_DLL_PATH ${TBB_DLL_PATH} $ENV{TBBROOT}/bin)
    FIND_PATH (TBB_DLL_DIR tbb.dll PATHS ${TBB_DLL_PATH})
    IF (${TBB_DLL_DIR} STREQUAL TBB_DLL_DIR-NOTFOUND)
        MESSAGE (STATUS "NOT Found TBB DLL")
        SET (TBB_FOUND FALSE)
    ELSE (${TBB_DLL_DIR} STREQUAL TBB_DLL_DIR-NOTFOUND)
        MESSAGE (STATUS "Found TBB DLL: ${TBB_DLL_DIR}/tbb.dll")
    ENDIF (${TBB_DLL_DIR} STREQUAL TBB_DLL_DIR-NOTFOUND)
ENDIF (MSVC)
