
find_package(PackageHandleStandardArgs)

find_path(DL_INCLUDE_DIR dlfcn.h libloaderapi.h
		PATHS ${CMAKE_SOURCE_DIR}
		PATH_SUFFIXES include external/include
		)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(DL_LIBRARY
						NAMES dl psapi
						PATH_SUFFIXES lib32 lib
						PATHS ${CMAKE_SOURCE_DIR}
						)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(DL_LIBRARY
						NAMES dl psapi
						PATH_SUFFIXES lib lib64
						PATHS ${CMAKE_SOURCE_DIR}
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
