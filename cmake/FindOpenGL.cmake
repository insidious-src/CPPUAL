find_package(PackageHandleStandardArgs)

find_path(GL_INCLUDE_DIR NAMES GL/gl.h GL/glew.h
			PATHS ${CMAKE_SOURCE_DIR}
			PATH_SUFFIXES include external/include
			)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(GL_LIBRARY
						NAMES GL opengl32
						PATH_SUFFIXES lib32 lib
						)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(GL_LIBRARY
						NAMES GL opengl64 opengl32
						PATH_SUFFIXES lib lib64
						)
endif()

find_package_handle_standard_args(OpenGL REQUIRED_VARS GL_LIBRARY GL_INCLUDE_DIR)

if(OPENGL_FOUND AND NOT TARGET OpenGL::Library)
		add_library(OpenGL::Library UNKNOWN IMPORTED)
		set_target_properties(OpenGL::Library PROPERTIES
				IMPORTED_LOCATION "${GL_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${GL_INCLUDE_DIR}"
				)
endif()

mark_as_advanced(GL_LIBRARY GL_INCLUDE_DIR)
