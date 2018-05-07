find_package(PackageHandleStandardArgs)

set(OPENCL_VERSION_STRING "0.1.0")
set(OPENCL_VERSION_MAJOR 0)
set(OPENCL_VERSION_MINOR 1)
set(OPENCL_VERSION_PATCH 0)

find_path(OPENCL_INCLUDE_DIR
  NAMES
    CL/cl.h OpenCL/cl.h
  PATHS
    ENV AMDAPPSDKROOT
    ENV INTELOCLSDKROOT
    ENV NVSDKCOMPUTE_ROOT
    ENV CUDA_PATH
    ENV ATISTREAMSDKROOT
  PATH_SUFFIXES
    include
    OpenCL/common/inc
    "AMD APP/include"
    )

find_path(_OPENCL_CPP_INCLUDE_DIR CL/cl.hpp OpenCL/cl.hpp DOC "Include for OpenCL CPP bindings"
        PATHS ${OPENCL_INCLUDE_DIR})

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(OPENCL_LIBRARY
      NAMES OpenCL
      HINTS
        ENV AMDAPPSDKROOT
        ENV INTELOCLSDKROOT
        ENV CUDA_PATH
        ENV NVSDKCOMPUTE_ROOT
        ENV ATISTREAMSDKROOT
      PATH_SUFFIXES
        "AMD APP/lib/x86"
        lib32
        lib
        lib/x86
        lib/Win32
        OpenCL/common/lib/Win32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(OPENCL_LIBRARY
      NAMES OpenCL
      HINTS
        ENV AMDAPPSDKROOT
        ENV INTELOCLSDKROOT
        ENV CUDA_PATH
        ENV NVSDKCOMPUTE_ROOT
        ENV ATISTREAMSDKROOT
      PATH_SUFFIXES
        "AMD APP/lib/x86_64"
        lib
        lib64
        lib/x86_64
        lib/x64
        OpenCL/common/lib/x64)
endif()

find_package_handle_standard_args(OpenCL REQUIRED_VARS OPENCL_LIBRARY OPENCL_INCLUDE_DIR)

if(_OPENCL_CPP_INCLUDE_DIR)
                set(OPENCL_HAS_CPP_BINDINGS TRUE)
endif(_OPENCL_CPP_INCLUDE_DIR)

if(OPENCL_FOUND AND NOT TARGET OpenCL::Library)
        add_library(OpenCL::Library UNKNOWN IMPORTED)
	set_target_properties(OpenCL::Library PROPERTIES
		IMPORTED_LOCATION "${OPENCL_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${OPENCL_INCLUDE_DIR}"
		)
endif()

mark_as_advanced(OPENCL_LIBRARY OPENCL_INCLUDE_DIR)
