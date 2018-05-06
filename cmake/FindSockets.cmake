find_package(PackageHandleStandardArgs)

set(HEADER_FILES     winsock.h winsock2.h arpa/inet.h sys/socket.h)
set(LIBRARY_NAMES    ws2_32 rt wsock32)
set(LIBRARY_SUFFIXES include/freetype2 include/freetype)

find_path(SOCKETS_INCLUDE_DIR NAMES ${HEADER_FILES}
        PATHS ${CMAKE_SOURCE_DIR}/external/include
        )

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(SOCKETS_LIBRARY
                     NAMES ${LIBRARY_NAMES}
                     PATH_SUFFIXES lib32 lib
                     )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(SOCKETS_LIBRARY
                     NAMES ${LIBRARY_NAMES}
                     PATH_SUFFIXES lib lib64
                     )
endif()

find_package_handle_standard_args(Sockets REQUIRED_VARS SOCKETS_LIBRARY SOCKETS_INCLUDE_DIR)

if(SOCKETS_FOUND AND NOT TARGET Sockets::Library)
        add_library(Sockets::Library UNKNOWN IMPORTED)
        set_target_properties(Sockets::Library PROPERTIES
                IMPORTED_LOCATION "${SOCKETS_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SOCKETS_INCLUDE_DIR}"
                )
endif()

mark_as_advanced(SOCKETS_LIBRARY SOCKETS_INCLUDE_DIR)
