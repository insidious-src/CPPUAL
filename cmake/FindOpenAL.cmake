find_package(PackageHandleStandardArgs)

set(HEADER_FILES    AL/al.h OpenAL/al.h)
set(LIBRARY_NAMES   openal OpenAL al OpenAL32 soft_oal)
set(LIBRARY_NAMES64 openal OpenAL al OpenAL64 soft_oal)

find_path(OPENAL_INCLUDE_DIR NAMES ${HEADER_FILES}
  HINTS
  ENV OPENALDIR
  PATH_SUFFIXES external/include include
  PATHS
  ${CMAKE_SOURCE_DIR}
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
)
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(OPENAL_LIBRARY
        NAMES ${LIBRARY_NAMES}
        HINTS ENV OPENALDIR
        PATH_SUFFIXES bin lib32 lib lib/Win32 libs/Win32 external/bin/x86
        PATHS
        ${CMAKE_FIND_ROOT_PATH}
        ${CMAKE_SOURCE_DIR}
        /usr
        ~/Library/Frameworks
        /Library/Frameworks
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
        NO_DEFAULT_PATH
        )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(OPENAL_LIBRARY
        NAMES ${LIBRARY_NAMES64}
        HINTS ENV OPENALDIR
        PATH_SUFFIXES bin lib lib64 lib/Win64 libs/Win64 external/bin/x86_64
        PATHS
        ${CMAKE_FIND_ROOT_PATH}
        ${CMAKE_SOURCE_DIR}
        /usr
        ~/Library/Frameworks
        /Library/Frameworks
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
        NO_DEFAULT_PATH
        )
endif()

find_package_handle_standard_args(OpenAL REQUIRED_VARS OPENAL_LIBRARY OPENAL_INCLUDE_DIR)

if(OPENAL_FOUND AND NOT TARGET OpenAL::Library)
    add_library(OpenAL::Library UNKNOWN IMPORTED)
    set_target_properties(OpenAL::Library PROPERTIES
        IMPORTED_LOCATION "${OPENAL_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}"
        )
endif()

mark_as_advanced(OPENAL_LIBRARY OPENAL_INCLUDE_DIR)
