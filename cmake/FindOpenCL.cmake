find_package(PackageHandleStandardArgs)

set(OPENCL_VERSION_STRING "0.1.0")
set(OPENCL_VERSION_MAJOR 0)
set(OPENCL_VERSION_MINOR 1)
set(OPENCL_VERSION_PATCH 0)

if(APPLE)
	find_library(OPENCL_LIBRARY OpenCL DOC "OpenCL lib for OSX")
	find_path(OPENCL_INCLUDE_DIRS OpenCL/cl.h DOC "Include for OpenCL on OSX")
	find_path(_OPENCL_CPP_INCLUDE_DIRS OpenCL/cl.hpp DOC "Include for OpenCL CPP bindings on OSX")
else(APPLE)
	if(WIN32)

                find_path(OPENCL_INCLUDE_DIRS CL/cl.h PATHS "$ENV{AMDAPPSDKROOT}/include")
                find_path(_OPENCL_CPP_INCLUDE_DIRS CL/cl.hpp PATHS "$ENV{AMDAPPSDKROOT}/include")

		# The AMD SDK currently installs both x86 and x86_64 libraries
		# This is only a hack to find out architecture
                if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                        find_library(OPENCL_LIBRARY OpenCL)
                elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
			find_library(OPENCL_LIBRARY OpenCL)
		endif()

	else(WIN32)

		# Unix style platforms
		find_library(OPENCL_LIBRARY OpenCL
			PATHS ENV LD_LIBRARY_PATH ENV OpenCL_LIBPATH
			)

		# The AMD SDK currently does not place its headers
		# in /usr/include, therefore also search relative
		# to the library
		find_path(OPENCL_INCLUDE_DIRS CL/cl.h
                        PATHS "/usr/local/cuda/include" "/opt/AMDAPP/include" ENV OpenCL_INCPATH
			)

		find_path(_OPENCL_CPP_INCLUDE_DIRS CL/cl.hpp
                        PATHS "/usr/local/cuda/include" "/opt/AMDAPP/include" ENV OpenCL_INCPATH
			)

	endif(WIN32)
endif(APPLE)

find_package_handle_standard_args(OpenCL REQUIRED_VARS OPENCL_LIBRARY OPENCL_INCLUDE_DIRS)

if(_OPENCL_CPP_INCLUDE_DIRS)
		set(OPENCL_HAS_CPP_BINDINGS TRUE)
		list(APPEND OPENCL_INCLUDE_DIRS ${_OPENCL_CPP_INCLUDE_DIRS})

		# This is often the same, so clean up
		list(REMOVE_DUPLICATES OPENCL_INCLUDE_DIRS)
endif(_OPENCL_CPP_INCLUDE_DIRS)

if(OPENCL_FOUND AND NOT TARGET OpenCL::Library)
        add_library(OpenCL::Library UNKNOWN IMPORTED)
	set_target_properties(OpenCL::Library PROPERTIES
		IMPORTED_LOCATION "${OPENCL_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${OPENCL_INCLUDE_DIRS}"
		)
endif()

mark_as_advanced(OPENCL_LIBRARY OPENCL_INCLUDE_DIRS)
