
find_package(PackageHandleStandardArgs)

find_path(SNDFILE_INCLUDE_DIR sndfile.h
	PATHS ${CMAKE_SOURCE_DIR}
	PATH_SUFFIXES external/include include
	)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	find_library(SNDFILE_LIBRARY
			NAMES libsndfile-1 sndfile-1 sndfile
                        PATH_SUFFIXES lib32 lib external/bin/x86
			PATHS ${CMAKE_SOURCE_DIR}
			)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	find_library(SNDFILE_LIBRARY
			NAMES libsndfile-1 sndfile-1 sndfile
                        PATH_SUFFIXES lib64 lib external/bin/x86_64
			PATHS ${CMAKE_SOURCE_DIR}
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
