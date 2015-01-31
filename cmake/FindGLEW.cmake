find_package(PackageHandleStandardArgs)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	find_path(GLEW_INCLUDE_DIR GL/glew.h
			PATHS ${CMAKE_SOURCE_DIR}/external/include NO_DEFAULT_PATH
			)

	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s
			PATHS ${CMAKE_SOURCE_DIR}/external/bin/x86 NO_DEFAULT_PATH
			)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s
			PATHS ${CMAKE_SOURCE_DIR}/external/bin/x86_64 NO_DEFAULT_PATH
			)
	endif()
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	find_path(GLEW_INCLUDE_DIR GL/glew.h
			PATH_SUFFIX include external/include
			PATHS ${CMAKE_SOURCE_DIR}
			)

	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s
			PATH_SUFFIX lib64 libs64 external/bin/x86
			PATHS ${CMAKE_SOURCE_DIR}
			)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s
			PATH_SUFFIX lib libs external/bin/x86_64
			PATHS ${CMAKE_SOURCE_DIR}
			)
	endif()
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR)

if(GLEW_FOUND AND NOT TARGET GLEW::Library)
	add_library(GLEW::Library SHARED IMPORTED)
	set_target_properties(GLEW::Library PROPERTIES
		IMPORTED_LOCATION "${GLEW_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}"
		)
endif()

mark_as_advanced(GLEW_LIBRARY GLEW_INCLUDE_DIR)
