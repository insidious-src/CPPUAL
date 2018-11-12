find_package(PackageHandleStandardArgs)

set(HEADER_FILES GL/glew.h)
set(LIBRARY_NAMES glew32 GLEW glew glew32s)
set(LIBRARY_NAMES64 glew64 GLEW glew glew64s)

find_path(GLEW_INCLUDE_DIR
          NAMES ${HEADER_FILES}
          HINTS ${CMAKE_SOURCE_DIR}/external/include
          )

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(GLEW_LIBRARY
                NAMES ${LIBRARY_NAMES}
                PATH_SUFFIXES bin lib32 lib external/bin/x86
                PATHS
                    ${CMAKE_FIND_ROOT_PATH}
                    ${CMAKE_SOURCE_DIR}
                    /usr
                NO_DEFAULT_PATH
                )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(GLEW_LIBRARY
                NAMES ${LIBRARY_NAMES64}
                PATH_SUFFIXES bin lib lib64 external/bin/x86_64
                PATHS
                    ${CMAKE_FIND_ROOT_PATH}
                    ${CMAKE_SOURCE_DIR}
                    /usr
                NO_DEFAULT_PATH
                )
endif()

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR)

if(GLEW_FOUND AND NOT TARGET GLEW::Library)
    add_library(GLEW::Library UNKNOWN IMPORTED)
    set_target_properties(GLEW::Library PROPERTIES
        IMPORTED_LOCATION "${GLEW_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
        )
endif()

mark_as_advanced(GLEW_LIBRARY GLEW_INCLUDE_DIR)
