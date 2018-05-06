find_package(PackageHandleStandardArgs)

set(HEADER_FILES  Vulkan/vulkan.h vulkan/vulkan.h)
set(LIBRARY_NAMES vk Vulkan vulkan vulkan-1)

set(VULKAN_VERSION_STRING "1.0.5")
set(VULKAN_VERSION_MAJOR 1)
set(VULKAN_VERSION_MINOR 0)
set(VULKAN_VERSION_PATCH 5)
set(CMAKE_FIND_LIBRARY_SUFFIXES .dll .so)

find_path(VULKAN_INCLUDE_DIR
  NAMES
    ${HEADER_FILES}
  PATHS
    ENV AMDAPPSDKROOT
    ENV INTELOCLSDKROOT
    ENV NVSDKCOMPUTE_ROOT
    ENV CUDA_PATH
    ENV ATISTREAMSDKROOT
  PATH_SUFFIXES
    ../Include
    include
    "AMD APP/include"
    )

find_path(_VULKAN_CPP_INCLUDE_DIR vulkan/spirv.hpp Vulkan/spirv.hpp DOC "Include for C++ SPIR-V"
        PATHS ${VULKAN_INCLUDE_DIR})

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(VULKAN_LIBRARY
      NAMES
        ${LIBRARY_NAMES}
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
        lib/Win32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(VULKAN_LIBRARY
      NAMES
        ${LIBRARY_NAMES}
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
        lib/x64)
endif()

find_package_handle_standard_args(Vulkan REQUIRED_VARS VULKAN_LIBRARY VULKAN_INCLUDE_DIR)

if(_VULKAN_CPP_INCLUDE_DIR)
                set(VULKAN_HAS_CPP_BINDINGS TRUE)
endif(_VULKAN_CPP_INCLUDE_DIR)

if(VULKAN_FOUND AND NOT TARGET Vulkan::Library)
        add_library(Vulkan::Library UNKNOWN IMPORTED)
        set_target_properties(Vulkan::Library PROPERTIES
                IMPORTED_LOCATION "${VULKAN_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_INCLUDE_DIR}"
        )
endif()

mark_as_advanced(VULKAN_LIBRARY VULKAN_INCLUDE_DIR)
