find_package(PackageHandleStandardArgs)

find_path(FREETYPE_INCLUDE_DIR NAMES freetype.h
		PATHS ${CMAKE_SOURCE_DIR}
		PATH_SUFFIXES include/freetype2 include/freetype include external/include
		)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	find_library(FREETYPE_LIBRARY
			NAMES freetype6 freetype freetype2
                        PATH_SUFFIXES lib32 lib external/bin/x86
			PATHS ${CMAKE_SOURCE_DIR}
			)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	find_library(FREETYPE_LIBRARY
			NAMES freetype6 freetype freetype2
                        PATH_SUFFIXES lib64 lib external/bin/x86_64
			PATHS ${CMAKE_SOURCE_DIR}
			)
endif()

find_package_handle_standard_args(Freetype REQUIRED_VARS FREETYPE_LIBRARY FREETYPE_INCLUDE_DIR)

if(FREETYPE_FOUND AND NOT TARGET Freetype::Library)
        add_library(Freetype::Library UNKNOWN IMPORTED)
	set_target_properties(Freetype::Library PROPERTIES
		IMPORTED_LOCATION "${FREETYPE_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
		)
endif()

mark_as_advanced(FREETYPE_LIBRARY FREETYPE_INCLUDE_DIR)
