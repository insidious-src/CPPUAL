
find_package(PackageHandleStandardArgs)

find_path(WM_INCLUDE_DIR wayland-client-protocol.h winuser.h
		PATHS ${CMAKE_SOURCE_DIR}
		PATH_SUFFIXES include external/include
		)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD")
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
			find_library(WM_LEGACY_LIBRARY xcb-icccm
					PATH_SUFFIXES lib32 lib
					)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
			find_library(WM_LEGACY_LIBRARY xcb-icccm
					PATH_SUFFIXES lib lib64
					)
	endif()

		find_package_handle_standard_args(WM_LEGACY REQUIRED_VARS WM_LEGACY_LIBRARY WM_INCLUDE_DIR)

		if(WM_LEGACY_FOUND AND NOT TARGET WM::LegacyLibrary)
						add_library(WM::LegacyLibrary UNKNOWN IMPORTED)
						set_target_properties(WM::LegacyLibrary PROPERTIES
										IMPORTED_LOCATION "${WM_LEGACY_LIBRARY}"
										INTERFACE_INCLUDE_DIRECTORIES "${WM_INCLUDE_DIR}"
										)
		endif()
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD")

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(WM_LIBRARY
						NAMES wayland-client user32
						PATH_SUFFIXES lib32 lib
						)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(WM_LIBRARY
						NAMES wayland-client user32
						PATH_SUFFIXES lib lib64
						)
endif()

find_package_handle_standard_args(WM REQUIRED_VARS WM_LIBRARY WM_INCLUDE_DIR)

if(WM_FOUND AND NOT TARGET WM::Library)
		add_library(WM::Library UNKNOWN IMPORTED)
		set_target_properties(WM::Library PROPERTIES
				IMPORTED_LOCATION "${WM_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${WM_INCLUDE_DIR}"
				)
endif()

mark_as_advanced(WM_LIBRARY WM_LEGACY_LIBRARY WM_INCLUDE_DIR)
