find_package(PackageHandleStandardArgs)

set(HEADER_FILES  sndfile.h)
set(LIBRARY_NAMES sndfile-1 sndfile)

find_path(SNDFILE_INCLUDE_DIR
    NAMES ${HEADER_FILES}
    PATH_SUFFIXES include external/include
    PATHS ${CMAKE_SOURCE_DIR}
    )

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(SNDFILE_LIBRARY
            NAMES ${LIBRARY_NAMES}
            PATH_SUFFIXES bin lib32 lib external/bin/x86
            PATHS
            ${CMAKE_FIND_ROOT_PATH}
            ${CMAKE_SOURCE_DIR}
            /usr
            NO_DEFAULT_PATH
            )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(SNDFILE_LIBRARY
            NAMES ${LIBRARY_NAMES}
            PATH_SUFFIXES bin lib lib64 external/bin/x86_64
            PATHS
            ${CMAKE_FIND_ROOT_PATH}
            ${CMAKE_SOURCE_DIR}
            /usr
            NO_DEFAULT_PATH
            )
endif()

find_package_handle_standard_args(SndFile REQUIRED_VARS SNDFILE_LIBRARY SNDFILE_INCLUDE_DIR)

if(SNDFILE_FOUND AND NOT TARGET SndFile::Library)
        add_library(SndFile::Library UNKNOWN IMPORTED)
    set_target_properties(SndFile::Library PROPERTIES
        IMPORTED_LOCATION "${SNDFILE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SNDFILE_INCLUDE_DIR}"
        )
endif()

mark_as_advanced(SNDFILE_LIBRARY SNDFILE_INCLUDE_DIR)
