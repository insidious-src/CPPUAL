find_package(PackageHandleStandardArgs)

set(DL_LIBRARIES dl libpsapi.dll psapi.dll psapi)

find_path(DL_INCLUDE_DIR
        NAMES
                dlfcn.h libloaderapi.h
        PATH_SUFFIXES
                include external/include
        )

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(DL_LIBRARY
                NAMES
                        ${DL_LIBRARIES}
                PATH_SUFFIXES
                        bin lib32 lib
                )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(DL_LIBRARY
                NAMES
                        ${DL_LIBRARIES}
                PATH_SUFFIXES
                        bin lib lib64
                )
endif()

find_package_handle_standard_args(DL REQUIRED_VARS DL_LIBRARY DL_INCLUDE_DIR)

if(DL_FOUND AND NOT TARGET DL::Library)
        add_library(DL::Library UNKNOWN IMPORTED)
        set_target_properties(DL::Library PROPERTIES
                IMPORTED_LOCATION "${DL_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DL_INCLUDE_DIR}"
                )
endif()

mark_as_advanced(DL_LIBRARY DL_INCLUDE_DIR)
