find_package(PackageHandleStandardArgs)

find_path(OPENAL_INCLUDE_DIR NAMES AL/al.h OpenAL/al.h
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
		NAMES OpenAL al openal OpenAL32 soft_oal
		HINTS ENV OPENALDIR
		PATH_SUFFIXES lib64 libs64 lib/Win32 libs/Win32 external/bin/x86
		PATHS
		${CMAKE_SOURCE_DIR}
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
		)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	find_library(OPENAL_LIBRARY
		NAMES OpenAL al openal OpenAL64 soft_oal
		HINTS ENV OPENALDIR
		PATH_SUFFIXES lib libs lib/Win64 libs/Win64 external/bin/x86_64
		PATHS
		${CMAKE_SOURCE_DIR}
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
		)
endif()

find_package_handle_standard_args(OpenAL REQUIRED_VARS OPENAL_LIBRARY OPENAL_INCLUDE_DIR)

if(OPENAL_FOUND AND NOT TARGET OpenAL::Library)
	add_library(OpenAL::Library SHARED IMPORTED)
	set_target_properties(OpenAL::Library PROPERTIES
		IMPORTED_LOCATION "${OPENAL_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}"
		)
endif()

mark_as_advanced(OPENAL_LIBRARY OPENAL_INCLUDE_DIR)
