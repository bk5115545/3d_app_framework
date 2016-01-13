
# This macro makes a list of the subdirectories relative to curdir
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(PACK_DIRS "${APP_PACK_DIR}")


# Add each pack to the build
# Each pack must use cmake and have a CMakeLists.txt
FOREACH(dir ${PACK_DIRS})
    add_subdirectory(${dir} EXPORT_VARIABLES PACK_SOURCE)
ENDFOREACH()
