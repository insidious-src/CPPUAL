cmake_minimum_required(VERSION 2.8.12)
find_package(PackageHandleStandardArgs)

set(EGL_HEADER_FILES EGL/egl.h)
set(EGL_LIBRARY_NAMES EGL)

if(NOT WIN32)

    # Use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    find_package(PkgConfig)
    pkg_check_modules(PKG_EGL QUIET egl)

        find_path(EGL_INCLUDE_DIR
            NAMES
                ${EGL_HEADER_FILES}
            HINTS
                ${PKG_EGL_INCLUDE_DIRS}
            PATH_SUFFIXES
                EGL
            )

    find_library(EGL_LIBRARY NAMES EGL HINTS ${PKG_EGL_LIBRARY_DIRS})

else()

    find_path(EGL_INCLUDE_DIR 
        NAMES ${EGL_HEADER_FILES}
        PATHS ${CMAKE_SOURCE_DIR}
        PATH_SUFFIXES external/include
        )

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(EGL_LIBRARY
            NAMES ${EGL_LIBRARY_NAMES}
            PATHS ${CMAKE_SOURCE_DIR}
            PATH_SUFFIXES bin/x86 external/bin/x86
            )
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(EGL_LIBRARY
            NAMES ${EGL_LIBRARY_NAMES}
            PATHS ${CMAKE_SOURCE_DIR}
            PATH_SUFFIXES bin/x86_64 external/bin/x86_64
            )
        endif()

endif()

# NB: We do *not* use the version information from pkg-config, as that
#     is the implementation version (eg: the Mesa version)
if(EGL_INCLUDE_DIR)
        # egl.h has defines of the form EGL_VERSION_x_y for each supported
        # version; so the header for EGL 1.1 will define EGL_VERSION_1_0 and
        # EGL_VERSION_1_1.  Finding the highest supported version involves
        # finding all these defines and selecting the highest numbered.
        file(READ "${EGL_INCLUDE_DIR}/EGL/egl.h" _EGL_header_contents)

        string(REGEX MATCHALL
                "[ \\t]EGL_VERSION_[0-9_]+"
                _EGL_version_lines
                "${_EGL_header_contents}"
        )

        unset(_EGL_header_contents)

        foreach(_EGL_version_line ${_EGL_version_lines})
                string(REGEX REPLACE
                        "[ \\t]EGL_VERSION_([0-9_]+)"
                        "\\1"
                        _version_candidate
                        "${_EGL_version_line}"
                )
                string(REPLACE "_" "." _version_candidate "${_version_candidate}")

                if(NOT DEFINED EGL_VERSION OR EGL_VERSION VERSION_LESS _version_candidate)
                        set(EGL_VERSION "${_version_candidate}")
                endif()
        endforeach()

        unset(_EGL_version_lines)
endif()

find_package_handle_standard_args(EGL
        FOUND_VAR
                EGL_FOUND
        REQUIRED_VARS
                EGL_LIBRARY
                EGL_INCLUDE_DIR
        VERSION_VAR
                EGL_VERSION
)

# compatibility variables
set(EGL_LIBRARIES ${EGL_LIBRARY})
set(EGL_INCLUDE_DIRS ${EGL_INCLUDE_DIR})
set(EGL_VERSION_STRING ${EGL_VERSION})

if(EGL_FOUND AND NOT TARGET EGL::Library)
        add_library(EGL::Library UNKNOWN IMPORTED)
        set_target_properties(EGL::Library PROPERTIES
                IMPORTED_LOCATION "${EGL_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${EGL_INCLUDE_DIR}"
        )
endif(EGL_FOUND AND NOT TARGET EGL::Library)

mark_as_advanced(EGL_LIBRARY EGL_INCLUDE_DIR)

include(FeatureSummary)
set_package_properties(EGL PROPERTIES
    URL "https://www.khronos.org/egl/"
    DESCRIPTION "A platform-agnostic mechanism for creating rendering surfaces for
                 use with other graphics libraries, such as OpenGL|ES and OpenVG."
)
