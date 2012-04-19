SET (vDist_FOUND TRUE)

FIND_PATH (vDist_INCLUDE_DIR vDist.hpp
    PATHS ${vDist_INC_PATH} ENV INCLUDE)
IF (${vDist_INCLUDE_DIR} STREQUAL vDist_INCLUDE_DIR-NOTFOUND)
    MESSAGE (STATUS "NOT Found vDist headers")
    SET (vDist_FOUND FALSE)
ELSE (${vDist_INCLUDE_DIR} STREQUAL vDist_INCLUDE_DIR-NOTFOUND)
    MESSAGE (STATUS "Found vDist headers: ${vDist_INCLUDE_DIR}")
ENDIF (${vDist_INCLUDE_DIR} STREQUAL vDist_INCLUDE_DIR-NOTFOUND)
