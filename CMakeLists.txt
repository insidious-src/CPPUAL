cmake_minimum_required(VERSION 3.10)

if(ANDROID)
    #if(NOT DEFINED ANDROID_NDK)
    #    set(ANDROID_NDK "/opt/android-ndk")
    #    set(ANDROID_NDK_ROOT "/opt/android-ndk")
    #endif()
    #set(ANDROID_SDK_ROOT "/opt/android-sdk")

    if (DEFINED ANDROID_ARCH64)
        set(ANDROID_ABI arm64-v8a)
    else ()
        set(ANDROID_ABI armeabi-v7a)
    endif ()

    if (DEFINED ANDROID_ARCH64)
        set(ANDROID_TOOLCHAIN_MACHINE_NAME "aarch64-linux-android")
    else ()
        set(ANDROID_TOOLCHAIN_MACHINE_NAME "arm-linux-androideabi")
    endif ()

    if (ANDROID_NATIVE_API_LEVEL MATCHES 16)
        set(ANDROID_PLATFORM "android-16")
    else ()
        set(ANDROID_PLATFORM "android-21")
        set(ANDROID_NATIVE_API_LEVEL 21)
    endif ()

    if(NOT DEFINED ANDROID_TOOLCHAIN)
        set(ANDROID_TOOLCHAIN "clang")
    endif()
    if(NOT DEFINED ANDROID_STL)
        set(ANDROID_STL c++_shared)
    endif()

    if(NOT DEFINED ANDROID_CPP_FEATURES)
        set(ANDROID_CPP_FEATURES "exceptions rtti")
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")
elseif(IOS_PLATFORM)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/ios.toolchain.cmake")
elseif(OSXCROSS)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-x86_64-apple-darwin15.cmake")
elseif(WIN32)
    set(TOOLCHAIN_PREFIX "i686-w64-mingw32")
    set(CMAKE_TOOLCHAIN_FILE "/usr/share/mingw/toolchain-${TOOLCHAIN_PREFIX}.cmake")
elseif(WIN64)
    set(TOOLCHAIN_PREFIX "x86_64-w64-mingw32")
    set(CMAKE_TOOLCHAIN_FILE "/usr/share/mingw/toolchain-${TOOLCHAIN_PREFIX}.cmake")
endif()

project("C++ Unified Abstraction Library" VERSION 1.0.5)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

# Enable unity builds globally
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 10)

set(LIBRARY_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR})

set(EXECUTABLE_OUTPUT_PATH "${LIBRARY_OUTPUT_PATH}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#include(CompilerConfig REQUIRED)
#include(CTest)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".DLL" ".a")
endif()

# ccache
find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

find_package(RT            REQUIRED) ## run-time library
find_package(DL            REQUIRED) ## dynamic loading of libraries
find_package(WM                    ) ## platform window manager
find_package(XCB COMPONENTS XCB ICCCM EWMH GLX) ## platform window manager (Xcb)
find_package(X11                   ) ## platform window manager (X11-xcb)
find_package(Sockets               ) ## network interface & interprocess
find_package(PulseAudio            )
find_package(OpenAL                ) ## will be dropped at some point
find_package(SndFile               ) ## will be dropped at some point
find_package(Freetype              ) ## will be dropped at some point
find_package(GLEW                  )
find_package(OpenGL                ) ## will be dropped at some point
find_package(EGL                   ) ## will be dropped at some point
find_package(OpenCL                ) ## backup compute api
find_package(Vulkan                ) ## primary compute & graphics api

include_directories(
    include
    RT::Library
    DL::Library
    OpenCL::Library
    Vulkan::Library
    Sockets::Library
    WM::Library
    EGL::Library
    GLEW::Library
    OpenGL::Library
    Freetype::Library
    SndFile::Library
    OpenAL::Library
    )


link_directories(
    ${PHOENIX_LIBRARIES_DIR}
    )

if(SOCKETS_FOUND)

### compile libraries
### device compute, memory (Vulkan, OpenCL, Serial)
set(CPPUAL_ENDOSKELETON_HEADERS
    "include/cppual/functional.h"
    "include/cppual/iterators/tuple_iterator.h"
    "include/cppual/iterators/bi_iterator.h"
    "include/cppual/iterators/rope_iterator.h"
    "include/cppual/bitfield.h"
    "include/cppual/fibers/fibers.h"
    "include/cppual/unbound_matrix.h"
    "include/cppual/bitset.h"
    "include/cppual/signal.h"
    "include/cppual/object.h"
    "include/cppual/types.h"
    "include/cppual/error.h"
    "include/cppual/common.h"
    "include/cppual/containers.h"
    "include/cppual/array_map.h"
    "include/cppual/string.h"
    "include/cppual/rope.h"
    "include/cppual/string_helper.h"
    "include/cppual/math.h"
    "include/cppual/reactive.h"
    "include/cppual/type_meta.h"
    "include/cppual/fn_meta.h"
    "include/cppual/str_meta.h"
    "include/cppual/concept/concepts.h"
    "include/cppual/circular_queue.h"
    "include/cppual/decl.h"
    "include/cppual/types.h"
    "include/cppual/passive_timeline.h"
    "include/cppual/noncopyable.h"
    "include/cppual/process/process_model.h"
    "include/cppual/process/interprocess.h"
    "include/cppual/process/details.h"
    "include/cppual/process/pipe.h"
    "include/cppual/process/plugin.h"
    "include/cppual/system/sysinfo.h"
    "include/cppual/system/clock.h"
    "include/cppual/exceptions/exception.h"
    "include/cppual/exceptions/functional_exception.h"
    "include/cppual/exceptions/memory_exception.h"
    "include/cppual/resource.h"
    "include/cppual/bigint.h"
    "include/cppual/byte.h"
    "include/cppual/interfaces/unbound_interface.h"
    "include/cppual/interfaces/layers.h"
    "include/cppual/forward.h"
    "include/cppual/memory/allocator.h"
    "include/cppual/memory/mop.h"
    "include/cppual/memory/stacked.h"
    "include/cppual/memory/pool.h"
    "include/cppual/memory/heap.h"
    "include/cppual/memory/page.h"
    "include/cppual/memory/shared.h"
    "include/cppual/abi.h"
    "include/cppual/compute/bridge.h"
    "include/cppual/compute/behaviour.h"
    "include/cppual/compute/object.h"
    "include/cppual/compute/backend_iface.h"
    #"include/cppual/compute/platform.h"
    "include/cppual/compute/program.h"
    "include/cppual/compute/device.h"
    "include/cppual/compute/devmemory.h"
    "include/cppual/compute/devtask.h"
    "include/cppual/compute/pll_ops.h"
    "include/cppual/compute/task.h"
    "include/cppual/compute/thread.h"

    "src/memory/os/linux.h"
    "src/memory/os/win.h"
    "src/memory/os/mac.h"
    "src/memory/os/android.h"
    "src/memory/os/ios.h"
    "src/memory/os/aix.h"
    "src/memory/os/solaris.h"
    "src/memory/os/bsd.h"
    )

# process, modules
add_library(cppual-endoskeleton SHARED
    "src/process/process_model.cpp"
    "src/process/interprocess.cpp"
    "src/process/plugin.cpp"
    "src/system/sysinfo.cpp"
    "src/interfaces/unbound_interface.cpp"
    "src/interfaces/layers.cpp"
    "src/memory/allocator.cpp"
    "src/memory/stacked.cpp"
    "src/memory/pool.cpp"
    "src/memory/heap.cpp"
    "src/memory/shared.cpp"
    "src/memory/page.cpp"
    "src/memory/shared.cpp"
    "src/compute/bridge.cpp"
    "src/compute/behaviour.cpp"
    "src/compute/object.cpp"
    "src/compute/backend_iface.cpp"
    "src/compute/program.cpp"
    "src/compute/device.cpp"
    "src/compute/devmemory.cpp"
    "src/compute/devtask.cpp"
    "src/compute/task.cpp"
    "src/compute/cv.cpp"
    "src/compute/mutex.cpp"
    "src/compute/thread.cpp"
    "src/compute/unbound_matrix.cpp"

    "include/cppual/decl"
    "include/cppual/types"
    "include/cppual/noncopyable"
    "include/cppual/abi"
    "include/cppual/casts"
    "include/cppual/meta_type"
    "include/cppual/meta_string"
    "include/cppual/meta_functional"
    "include/cppual/plugin"
    "include/cppual/resource"
    "include/cppual/timer"
    "include/cppual/system_info"
    "include/cppual/bigint"
    "include/cppual/circular_queue"
    "include/cppual/concepts"
    "include/cppual/forward"
    "include/cppual/iterator"
    "include/cppual/containers"
    "include/cppual/array_map"
    "include/cppual/string"
    "include/cppual/bitflags"
    "include/cppual/functional"
    "include/cppual/signal"
    "include/cppual/meta"
    "include/cppual/exception"
    "include/cppual/reactive"
    "include/cppual/byte"
    "include/cppual/interface"
    "include/cppual/unbound_matrix"
    "include/cppual/memory_resource"
    "include/cppual/memory_allocator"
    "include/cppual/shared_memory"
    "include/cppual/parallelism"
    "include/cppual/thread"
    "include/cppual/mutex"
    )
target_compile_definitions(cppual-endoskeleton PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-endoskeleton PRIVATE ${CPPUAL_ENDOSKELETON_HEADERS})

endif ()

# memory management
set(CPPUAL_MEMORY_SYSTEM_HEADERS
    "include/cppual/memory/system.h"
    )

add_library(cppual-memory-system SHARED
    "src/memory/system.cpp"

    "include/cppual/system_memory"
    )
target_compile_definitions(cppual-memory-system PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-memory-system PRIVATE ${CPPUAL_MEMORY_SYSTEM_HEADERS})

#if (NOT ANDROID)
#    target_compile_definitions(cppual-memory-system PRIVATE -DCPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY)
#endif()

set(CPPUAL_FS_HEADERS
    "include/cppual/filesystem/file.h"
    "include/cppual/filesystem/directory.h"
    "include/cppual/filesystem/directory_watcher.h"
    )

add_library(cppual-filesystem SHARED
    "src/filesystem/file.cpp"
    "src/filesystem/directory.cpp"
    "src/filesystem/directory_watcher.cpp"

    "include/cppual/filesystem_file"
    "include/cppual/filesystem_directory"
    "include/cppual/filesystem_watcher"
    )
target_compile_definitions(cppual-filesystem PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-filesystem PRIVATE ${CPPUAL_FS_HEADERS})

# json
set(CPPUAL_JSON_HEADERS
    "include/cppual/json/error/en.h"
    "include/cppual/json/error/error.h"
    "include/cppual/json/internal/biginteger.h"
    "include/cppual/json/internal/clzll.h"
    "include/cppual/json/internal/diyfp.h"
    "include/cppual/json/internal/dtoa.h"
    "include/cppual/json/internal/ieee754.h"
    "include/cppual/json/internal/itoa.h"
    "include/cppual/json/internal/meta.h"
    "include/cppual/json/internal/pow10.h"
    "include/cppual/json/internal/regex.h"
    "include/cppual/json/internal/stack.h"
    "include/cppual/json/internal/strfunc.h"
    "include/cppual/json/internal/strtod.h"
    "include/cppual/json/internal/swap.h"
    "include/cppual/json/allocators.h"
    "include/cppual/json/cursorstreamwrapper.h"
    "include/cppual/json/document.h"
    "include/cppual/json/encodedstream.h"
    "include/cppual/json/encodings.h"
    "include/cppual/json/filereadstream.h"
    "include/cppual/json/filewritestream.h"
    "include/cppual/json/fwd.h"
    "include/cppual/json/istreamwrapper.h"
    "include/cppual/json/rapidjson.h"
    "include/cppual/json/memorybuffer.h"
    "include/cppual/json/memorystream.h"
    "include/cppual/json/ostreamwrapper.h"
    "include/cppual/json/pointer.h"
    "include/cppual/json/prettywriter.h"
    "include/cppual/json/reader.h"
    "include/cppual/json/schema.h"
    "include/cppual/json/stream.h"
    "include/cppual/json/stringbuffer.h"
    "include/cppual/json/writer.h"
    "include/cppual/json/json_adaptor.h"
    )

add_library(cppual-json SHARED
    "src/json/json_adaptor.cpp"
    "include/cppual/json/msinttypes/inttypes.h"
    "include/cppual/json/msinttypes/stdint.h"

    "include/cppual/json_parser"
    )
target_compile_definitions(cppual-json PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-json PRIVATE ${CPPUAL_JSON_HEADERS})

if(VULKAN_FOUND)

set(CPPUAL_COMPUTE_VK_HEADERS
    "src/compute/backend/vulkan/vulkan.h"
    )

add_library(cppual-compute-plugin-vulkan MODULE
    "src/compute/backend/vulkan/vulkan.cpp"
    )
target_precompile_headers(cppual-compute-plugin-vulkan PRIVATE ${CPPUAL_COMPUTE_VK_HEADERS})

endif()

if(OPENCL_FOUND)

set(CPPUAL_COMPUTE_CL_HEADERS
    "src/compute/backend/opencl/opencl.h"
    "src/compute/backend/opencl/cldevice.h"
    "src/compute/backend/opencl/clprogram.h"
    "src/compute/backend/opencl/clkernel.h"
    "src/compute/backend/opencl/clsampler.h"
    "src/compute/backend/opencl/clcontext.h"
    "src/compute/backend/opencl/clevent.h"
    "src/compute/backend/opencl/clcmdqueue.h"
    "src/compute/backend/opencl/clbuffer.h"
    "src/compute/backend/opencl/climage.h"
    )

add_library(cppual-compute-plugin-opencl MODULE
    "src/compute/backend/opencl/opencl.cpp"
    "src/compute/backend/opencl/cldevice.cpp"
    "src/compute/backend/opencl/clprogram.cpp"
    "src/compute/backend/opencl/clkernel.cpp"
    "src/compute/backend/opencl/clsampler.cpp"
    "src/compute/backend/opencl/clcontext.cpp"
    "src/compute/backend/opencl/clevent.cpp"
    "src/compute/backend/opencl/clcmdqueue.cpp"
    "src/compute/backend/opencl/clbuffer.cpp"
    "src/compute/backend/opencl/climage.cpp"
    "src/compute/backend/opencl/clentry.cpp"
    )
target_precompile_headers(cppual-compute-plugin-opencl PRIVATE ${CPPUAL_COMPUTE_CL_HEADERS})

endif()

#add_library(cppual-db SHARED
#	"include/cppual/db/model.h"
#   "include/cppual/database"
#	)

# Document Object Model (xml, cmdargs, ini, json)
set(CPPUAL_DOM_HEADERS
    "include/cppual/dom/cmdline.h"
    "include/cppual/dom/csv.h"

#    "include/cppual/dom/exceptions.h"
#    "include/cppual/dom/id_translator.h"
#    "include/cppual/dom/ini_parser.h"
#    "include/cppual/dom/json_parser.h"
#    "include/cppual/dom/ptree_fwd.h"
#    "include/cppual/dom/ptree_serialization.h"
#    "include/cppual/dom/ptree.h"
#    "include/cppual/dom/stream_translator.h"
#    "include/cppual/dom/string_path.h"
#    "include/cppual/dom/xml_parser.h"

#    "include/cppual/dom/json_parser/error.h"
#    "include/cppual/dom/json_parser/detail/narrow_encoding.h"
#    "include/cppual/dom/json_parser/detail/parser.h"
#    "include/cppual/dom/json_parser/detail/read.h"
#    "include/cppual/dom/json_parser/detail/standard_callbacks.h"
#    "include/cppual/dom/json_parser/detail/wide_encoding.h"
#    "include/cppual/dom/json_parser/detail/write.h"

#    "include/cppual/dom/detail/exception_implementation.h"
#    "include/cppual/dom/detail/file_parser_error.h"
#    "include/cppual/dom/detail/ptree_implementation.h"
#    "include/cppual/dom/detail/ptree_utils.h"
#    "include/cppual/dom/detail/rapidxml.h"
#    "include/cppual/dom/detail/xml_parser_error.h"
#    "include/cppual/dom/detail/xml_parser_flags.h"
#    "include/cppual/dom/detail/xml_parser_read_rapidxml.h"
#    "include/cppual/dom/detail/xml_parser_utils.h"
#    "include/cppual/dom/detail/xml_parser_write.h"
#    "include/cppual/dom/detail/xml_parser_writer_settings.h"
    )

add_library(cppual-dom MODULE
    "src/dom/csv.cpp"

    "include/cppual/dom_parser"
    "include/cppual/ini_parser"
    "include/cppual/csv_parser"
    "include/cppual/json_parser"
    "include/cppual/xml_parser"
    )
target_compile_definitions(cppual-dom PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-dom PRIVATE ${CPPUAL_DOM_HEADERS})

# transparent network facilities
set(CPPUAL_NET_HEADERS
    "include/cppual/network/address.h"
    "include/cppual/network/packet.h"
    "include/cppual/network/uri.h"
    "include/cppual/network/uri_parts.h"
    "include/cppual/network/transport/socket.h"
    "include/cppual/network/transport/tcp.h"
    "include/cppual/network/transport/tcpclient.h"
    "include/cppual/network/transport/tcplistener.h"
    "include/cppual/network/transport/udp.h"
    "include/cppual/network/transport/proxy.h"
    "include/cppual/network/protocols/ftp.h"
    "include/cppual/network/protocols/http.h"
    "include/cppual/network/protocols/p2p.h"
    "include/cppual/network/protocols/metalink.h"
    "include/cppual/network/protocols/ssh.h"
    "include/cppual/network/protocols/protocol.h"
    "include/cppual/network/protocols/server_protocol.h"

    "src/network/transport/os/linux.h"
    "src/network/transport/os/win.h"
    "src/network/transport/os/mac.h"
    "src/network/transport/os/android.h"
    "src/network/transport/os/ios.h"
    "src/network/transport/os/aix.h"
    "src/network/transport/os/solaris.h"
    "src/network/transport/os/bsd.h"
    )

if(SOCKETS_FOUND)

add_library(cppual-network MODULE
    "src/network/address.cpp"
    "src/network/packet.cpp"
    "src/network/uri.cpp"
    "src/network/transport/socket.cpp"
    "src/network/transport/tcp.cpp"
    "src/network/transport/tcpclient.cpp"
    "src/network/transport/tcplistener.cpp"
    "src/network/transport/udp.cpp"
    "src/network/transport/proxy.cpp"
    "src/network/protocols/ftp.cpp"
    "src/network/protocols/http.cpp"
    "src/network/protocols/p2p.cpp"
    "src/network/protocols/metalink.cpp"
    "src/network/protocols/ssh.cpp"
    "src/network/protocols/protocol.cpp"
    "src/network/protocols/server_protocol.cpp"

    "include/cppual/network_protocol"
    "include/cppual/network_basic_protocol"
    "include/cppual/network_http"
    "include/cppual/network_ftp"
    "include/cppual/network_p2p"
    "include/cppual/network_metalink"
    "include/cppual/network_ssh"
    "include/cppual/network_proxy"
    )
target_compile_definitions(cppual-network PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-network PRIVATE ${CPPUAL_NET_HEADERS})

endif()

# raw device input
set(CPPUAL_INPUT_HEADERS
    "include/cppual/input/event.h"
    "include/cppual/input/keyboard.h"
    "include/cppual/input/pointer.h"
    "include/cppual/input/joystick.h"
    "include/cppual/input/joysticks/joy_generic.h"
    "include/cppual/input/joysticks/joy_ps.h"
    "include/cppual/input/joysticks/joy_steam.h"
    "include/cppual/input/joysticks/joy_xbox.h"
    "include/cppual/input/joysticks/joy_wheel.h"
    )

add_library(cppual-input MODULE
    "src/input/keyboard.cpp"
    "src/input/pointer.cpp"
    "src/input/joystick.cpp"

    "include/cppual/input_devices"
    "include/cppual/input_event"
    )
target_compile_definitions(cppual-input PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-input PRIVATE ${CPPUAL_INPUT_HEADERS})

# mqtt client implementation
set(CPPUAL_MQTT_HEADERS
    "include/cppual/mqtt/client.h"
    "include/cppual/mqtt/message.h"
    )

add_library(cppual-mqtt MODULE
    "src/mqtt/client.cpp"
    "src/mqtt/message.cpp"

    "include/cppual/mqtt_client"
    )
target_compile_definitions(cppual-mqtt PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-mqtt PRIVATE ${CPPUAL_MQTT_HEADERS})

if(SNDFILE_FOUND)

# audio library (OpenAL, TrueAudio, Compute)
set(CPPUAL_AUDIO_HEADERS
    "include/cppual/multimedia/audio/sound.h"
    "include/cppual/multimedia/audio/spatial.h"
    "include/cppual/multimedia/audio/format.h"
    "include/cppual/multimedia/audio/device.h"
    "include/cppual/multimedia/audio/buffer.h"
    "include/cppual/multimedia/audio/source.h"
    "include/cppual/multimedia/audio/emitter.h"
    "include/cppual/multimedia/audio/playback.h"
    "include/cppual/multimedia/audio/listener.h"
    "include/cppual/multimedia/audio/record.h"
    )

add_library(cppual-audio SHARED
    "src/multimedia/audio/sound.cpp"
    "src/multimedia/audio/spatial.cpp"
    "src/multimedia/audio/format.cpp"
    "src/multimedia/audio/device.cpp"
    "src/multimedia/audio/buffer.cpp"
    "src/multimedia/audio/source.cpp"
    "src/multimedia/audio/emitter.cpp"
    "src/multimedia/audio/playback.cpp"
    "src/multimedia/audio/listener.cpp"
    "src/multimedia/audio/record.cpp"

    "include/cppual/audio"
    )
target_compile_definitions(cppual-audio PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-audio PRIVATE ${CPPUAL_AUDIO_HEADERS})

endif()

#if(TARGET cppual-audio AND OPENAL_FOUND)
if(OPENAL_FOUND)

# audio backend OpenAL
set(CPPUAL_AUDIO_AL_HEADERS
    "src/multimedia/audio/al/aldef.h"
    "include/cppual/multimedia/audio/al/alobject.h"
    "include/cppual/multimedia/audio/al/aldevice.h"
    "include/cppual/multimedia/audio/al/albuffer.h"
    "include/cppual/multimedia/audio/al/alsource.h"
    "include/cppual/multimedia/audio/al/emitter.h"
    "include/cppual/multimedia/audio/al/music.h"
    "include/cppual/multimedia/audio/al/listener.h"
    "include/cppual/multimedia/audio/al/recorder.h"
    )

add_library(cppual-audio-plugin-openal MODULE
    "src/multimedia/audio/al/alobject.cpp"
    "src/multimedia/audio/al/aldevice.cpp"
    "src/multimedia/audio/al/albuffer.cpp"
    "src/multimedia/audio/al/alsource.cpp"
    "src/multimedia/audio/al/emitter.cpp"
    "src/multimedia/audio/al/music.cpp"
    "src/multimedia/audio/al/listener.cpp"
    "src/multimedia/audio/al/recorder.cpp"
    "src/multimedia/audio/backend/al/al_main.cpp"

    "include/cppual/openal"
    )
target_compile_definitions(cppual-audio-plugin-openal PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-audio-plugin-openal PRIVATE ${CPPUAL_AUDIO_AL_HEADERS})

endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND PulseAudio_FOUND)

set(CPPUAL_AUDIO_COMPUTE_HEADERS
    "src/multimedia/audio/backend/compute/linux/pulse.h"
    "src/multimedia/audio/backend/compute/linux/pulse_device.h"
    )

add_library(cppual-audio-plugin-compute MODULE
    "src/multimedia/audio/backend/compute/linux/pulse_device.cpp"
    "src/multimedia/audio/backend/compute/compute_main.cpp"
    )
target_precompile_headers(cppual-audio-plugin-compute PRIVATE ${CPPUAL_AUDIO_COMPUTE_HEADERS})

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(CPPUAL_AUDIO_COMPUTE_WIN_HEADERS
    "src/multimedia/audio/backend/compute/win/win.h"
    "src/multimedia/audio/backend/compute/win/win_device.h"
    )

add_library(cppual-audio-plugin-compute MODULE
    "src/multimedia/audio/backend/compute/win/win_device.cpp"
    "src/multimedia/audio/backend/compute/compute_main.cpp"
    )
target_precompile_headers(cppual-audio-plugin-compute PRIVATE ${CPPUAL_AUDIO_COMPUTE_WIN_HEADERS})

endif()

# multimedia
set(CPPUAL_MULTIMEDIA_HEADERS
    "include/cppual/multimedia/media_mgr.h"
    "include/cppual/multimedia/media_sync.h"
    )

add_library(cppual-multimedia MODULE
    "src/multimedia/media_mgr.cpp"
    "src/multimedia/media_sync.cpp"
    )
target_compile_definitions(cppual-multimedia PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-multimedia PRIVATE ${CPPUAL_MULTIMEDIA_HEADERS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# video acceleration api backend
set(CPPUAL_VIDEO_VAAPI_HEADERS
    )

add_library(cppual-video-plugin-vaapi MODULE
    "src/multimedia/video/backend/vaapi.cpp"
    )
target_precompile_headers(cppual-video-plugin-vaapi PRIVATE ${CPPUAL_VIDEO_VAAPI_HEADERS})

# Xorg video backend
set(CPPUAL_VIDEO_XORG_HEADERS
    )

add_library(cppual-video-plugin-xorg MODULE
    "src/multimedia/video/backend/xvideo.cpp"
    )
target_precompile_headers(cppual-video-plugin-xorg PRIVATE ${CPPUAL_VIDEO_XORG_HEADERS})

endif()

# compute video backend
set(CPPUAL_VIDEO_COMPUTE_HEADERS
    )

if(TARGET cppual-endoskeleton)

add_library(cppual-video-plugin-compute MODULE
    "src/multimedia/video/backend/compute_video.cpp"
    )
target_precompile_headers(cppual-video-plugin-compute PRIVATE ${CPPUAL_VIDEO_COMPUTE_HEADERS})

endif()


# video library (VAAPI, XVideo, OpenGL)
set(CPPUAL_VIDEO_HEADERS
    "include/cppual/multimedia/video/decoder.h"
    "include/cppual/multimedia/video/vfilter.h"
    "include/cppual/multimedia/video/vequalizer.h"
    )

add_library(cppual-video MODULE
    "src/multimedia/video/vfilter.cpp"
    "src/multimedia/video/vequalizer.cpp"

    "include/cppual/video"
    )
target_compile_definitions(cppual-video PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-video PRIVATE ${CPPUAL_VIDEO_HEADERS})

# graphics drawing library
set(CPPUAL_GFX_HEADERS
    "include/cppual/gfx/coord.h"
    "include/cppual/gfx/color.h"
    "include/cppual/gfx/matrix.h"
    "include/cppual/gfx/draw.h"
    "include/cppual/gfx/formats/bmp.h"
    "include/cppual/gfx/drawable/shape.h"
    "include/cppual/gfx/drawable/image.h"
    )

add_library(cppual-gfx SHARED
    "src/gfx/color.cpp"
    "src/gfx/matrix.cpp"
    "src/gfx/draw.cpp"
    "src/gfx/formats/bmp.cpp"
    "src/gfx/drawable/image.cpp"

    "include/cppual/graphics"
    )
target_compile_definitions(cppual-gfx PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-gfx PRIVATE ${CPPUAL_GFX_HEADERS})

if(TARGET cppual-gfx AND FREETYPE_FOUND)

set(CPPUAL_GFX_FONT_HEADERS
    "include/cppual/gfx/font.h"
    )

add_library (cppual-gfx-font SHARED
    "src/gfx/font.cpp"
    )
target_compile_definitions(cppual-gfx-font PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-gfx-font PRIVATE ${CPPUAL_GFX_FONT_HEADERS})

endif()

if(TARGET cppual-gfx AND OPENGL_FOUND AND GLEW_FOUND)

# OpenGL/ES library
set(CPPUAL_GL_HEADERS
    #"include/cppual/gfx/gl/glad.h"
    #"include/cppual/gfx/gl/khrplatform.h"
    "include/cppual/gfx/gl/gldef.h"
    "include/cppual/gfx/gl/glbase.h"
    "include/cppual/gfx/gl/runtime.h"
    "include/cppual/gfx/gl/glsl.h"
    "include/cppual/gfx/gl/texture.h"
    "include/cppual/gfx/gl/buffer.h"
    "include/cppual/gfx/gl/vbo.h"
    "include/cppual/gfx/gl/framebuf.h"
    "include/cppual/gfx/gl/query.h"
    "include/cppual/gfx/gl/batch.h"
    "include/cppual/gfx/gl/instance.h"
    "include/cppual/gfx/gl/painter.h"
    "include/cppual/gfx/gl/text.h"
    "include/cppual/gfx/gl/elipse.h"
    "include/cppual/gfx/gl/rectangle.h"
    "include/cppual/gfx/gl/polygon.h"
    "include/cppual/gfx/gl/line.h"
    )

add_library(cppual-gl SHARED
    "src/gfx/gl/glad.c"
    "src/gfx/gl/glbase.cpp"
    "src/gfx/gl/runtime.cpp"
    "src/gfx/gl/glsl.cpp"
    "src/gfx/gl/texture.cpp"
    "src/gfx/gl/buffer.cpp"
    "src/gfx/gl/vbo.cpp"
    "src/gfx/gl/framebuf.cpp"
    "src/gfx/gl/query.cpp"
    "src/gfx/gl/batch.cpp"
    "src/gfx/gl/instance.cpp"
    "src/gfx/gl/painter.cpp"
    "src/gfx/gl/text.cpp"
    "src/gfx/gl/elipse.cpp"
    "src/gfx/gl/rectangle.cpp"
    "src/gfx/gl/polygon.cpp"
    "src/gfx/gl/line.cpp"

    "include/cppual/opengl"
    )
target_compile_definitions(cppual-gl PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-gl PRIVATE ${CPPUAL_GL_HEADERS})

endif()

if(TARGET cppual-gfx)

    if(TARGET cppual-gl)
        if(EGL_FOUND)
            set(CPPUAL_GFX_EGL_HEADERS
                "src/gfx/backend/egl/egl_surface.h"
                )

            add_library(cppual-gfx-plugin-egl MODULE
                "src/gfx/backend/egl/egl_surface.cpp"
                "src/gfx/backend/egl/egl_main.cpp"
                )
            target_precompile_headers(cppual-gfx-plugin-egl PRIVATE ${CPPUAL_GFX_EGL_HEADERS})
        endif()

        if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND XCB_GLX_FOUND)
            set(CPPUAL_GFX_GLX_HEADERS
                "src/gfx/backend/glx/glx_surface.h"
                )

            add_library(cppual-gfx-plugin-glx MODULE
                "src/gfx/backend/glx/glx_main.cpp"
                "src/gfx/backend/glx/glx_surface.cpp"
                )
            target_precompile_headers(cppual-gfx-plugin-glx PRIVATE ${CPPUAL_GFX_GLX_HEADERS})
        endif()
    endif()

    if(VULKAN_FOUND)
        set(CPPUAL_GFX_VK_HEADERS
            )

        add_library(cppual-gfx-plugin-vulkan MODULE
            "src/gfx/backend/vulkan/vk_main.cpp"
            )
        target_precompile_headers(cppual-gfx-plugin-vulkan PRIVATE ${CPPUAL_GFX_VK_HEADERS})
    endif()


    # graphical user interface
    set(CPPUAL_UI_HEADERS
        "include/cppual/ui/manager.h"
        "include/cppual/ui/display.h"
        "include/cppual/ui/queue.h"
        "include/cppual/ui/wm.h"
        "include/cppual/ui/vsurface.h"
        "include/cppual/ui/animation.h"
        "include/cppual/ui/composite.h"
        "include/cppual/ui/view.h"
        "include/cppual/ui/skin.h"
        "include/cppual/ui/layout.h"
        "include/cppual/ui/hotkey.h"
        "include/cppual/ui/cmd.h"
        "include/cppual/ui/groups.h"
        )

    add_library(cppual-ui SHARED
        "src/ui/manager.cpp"
        "src/ui/display.cpp"
        "src/ui/queue.cpp"
        "src/ui/vsurface.cpp"
        "src/ui/animation.cpp"
        "src/ui/composite.cpp"
        "src/ui/view.cpp"
        "src/ui/skin.cpp"
        "src/ui/layout.cpp"
        "src/ui/hotkey.cpp"
        #"src/ui/cmd.cpp""
        #"src/ui/groups.cpp"

        "include/cppual/ui_queue"
        "include/cppual/ui_view"
        "include/cppual/ui_layouts"
        "include/cppual/ui_hotkey"
        )
    target_compile_definitions(cppual-ui PRIVATE -DAPI_EXPORT)
    target_precompile_headers(cppual-ui PRIVATE ${CPPUAL_UI_HEADERS})

endif()




if (TARGET cppual-ui)

# graphical user interface - windows and controls
set(CPPUAL_UI_VIEWS_HEADERS
    "include/cppual/ui/window.h"
    "include/cppual/ui/frame.h"
    "include/cppual/ui/dialog.h"
    "include/cppual/ui/controls/label.h"
    "include/cppual/ui/controls/button.h"
    "include/cppual/ui/controls/popup.h"
    "include/cppual/ui/controls/container.h"
    "include/cppual/ui/controls/mdi.h"
    "include/cppual/ui/controls/toolbar.h"
    "include/cppual/ui/controls/scrollbar.h"
    "include/cppual/ui/controls/roller.h"
    "include/cppual/ui/controls/progress.h"
    "include/cppual/ui/controls/combo.h"
    "include/cppual/ui/controls/groupbox.h"
    "include/cppual/ui/controls/tab.h"
    "include/cppual/ui/controls/tooltip.h"
    "include/cppual/ui/controls/tree.h"
    "include/cppual/ui/controls/input.h"
    "include/cppual/ui/controls/graph.h"
    "include/cppual/ui/controls/listbox.h"
    "include/cppual/ui/controls/slider.h"
    "include/cppual/ui/controls/abslider.h"
    )

add_library(cppual-ui-views SHARED
    "src/ui/window.cpp"
    "src/ui/frame.cpp"
    "src/ui/dialog.cpp"
    "src/ui/controls/label.cpp"
    "src/ui/controls/button.cpp"
    "src/ui/controls/popup.cpp"

    "include/cppual/ui_window"
    "include/cppual/ui_controls"
    )
target_compile_definitions(cppual-ui-views PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-ui-views PRIVATE ${CPPUAL_UI_VIEWS_HEADERS})

# graphical user interface - native dialogs
set(CPPUAL_UI_DLG_HEADERS
    "include/cppual/ui/dialogs/msgdlg.h"
    "include/cppual/ui/dialogs/colordlg.h"
    "include/cppual/ui/dialogs/filedlg.h"
    "include/cppual/ui/dialogs/printdlg.h"
    "include/cppual/ui/dialogs/taskdlg.h"
    "include/cppual/ui/dialogs/wizarddlg.h"
    )

add_library(cppual-ui-dialogs MODULE
    "src/ui/dialogs/msgdlg.cpp"

    "include/cppual/ui_dialogs"
    )
target_compile_definitions(cppual-ui-dialogs PRIVATE -DAPI_EXPORT)
target_precompile_headers(cppual-ui-dialogs PRIVATE ${CPPUAL_UI_DLG_HEADERS})

endif()

if(TARGET cppual-ui AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(XCB_EWMH_FOUND AND XCB_ICCCM_FOUND AND X11_FOUND)

# graphical user interface - platform Xorg
set(CPPUAL_UI_XORG_HEADERS
    "include/cppual/ui/platforms/xcb/atom.hpp"
    "include/cppual/ui/platforms/xcb/colormap.hpp"
    "include/cppual/ui/platforms/xcb/connection.hpp"
    "include/cppual/ui/platforms/xcb/core.hpp"
    "include/cppual/ui/platforms/xcb/cursor.hpp"
    "include/cppual/ui/platforms/xcb/drawable.hpp"
    "include/cppual/ui/platforms/xcb/event.hpp"
    "include/cppual/ui/platforms/xcb/font.hpp"
    "include/cppual/ui/platforms/xcb/fontable.hpp"
    "include/cppual/ui/platforms/xcb/gcontext.hpp"
    "include/cppual/ui/platforms/xcb/generic/error.hpp"
    "include/cppual/ui/platforms/xcb/generic/event.hpp"
    "include/cppual/ui/platforms/xcb/generic/extension.hpp"
    "include/cppual/ui/platforms/xcb/generic/factory.hpp"
    "include/cppual/ui/platforms/xcb/generic/input_iterator_adapter.hpp"
    "include/cppual/ui/platforms/xcb/generic/iterator_traits.hpp"
    "include/cppual/ui/platforms/xcb/generic/reply_iterator.hpp"
    "include/cppual/ui/platforms/xcb/generic/request.hpp"
    "include/cppual/ui/platforms/xcb/generic/resource.hpp"
    "include/cppual/ui/platforms/xcb/generic/signature.hpp"
    "include/cppual/ui/platforms/xcb/generic.hpp"
    "include/cppual/ui/platforms/xcb/pixmap.hpp"
    "include/cppual/ui/platforms/xcb/proto/bigreq.hpp"
    "include/cppual/ui/platforms/xcb/proto/composite.hpp"
    "include/cppual/ui/platforms/xcb/proto/damage.hpp"
    "include/cppual/ui/platforms/xcb/proto/dpms.hpp"
    "include/cppual/ui/platforms/xcb/proto/dri2.hpp"
    "include/cppual/ui/platforms/xcb/proto/dri3.hpp"
    "include/cppual/ui/platforms/xcb/proto/glx.hpp"
    "include/cppual/ui/platforms/xcb/proto/present.hpp"
    "include/cppual/ui/platforms/xcb/proto/randr.hpp"
    "include/cppual/ui/platforms/xcb/proto/record.hpp"
    "include/cppual/ui/platforms/xcb/proto/render.hpp"
    "include/cppual/ui/platforms/xcb/proto/res.hpp"
    "include/cppual/ui/platforms/xcb/proto/screensaver.hpp"
    "include/cppual/ui/platforms/xcb/proto/shape.hpp"
    "include/cppual/ui/platforms/xcb/proto/shm.hpp"
    "include/cppual/ui/platforms/xcb/proto/sync.hpp"
    "include/cppual/ui/platforms/xcb/proto/x.hpp"
    "include/cppual/ui/platforms/xcb/proto/xc_misc.hpp"
    "include/cppual/ui/platforms/xcb/proto/xevie.hpp"
    "include/cppual/ui/platforms/xcb/proto/xf86dri.hpp"
    "include/cppual/ui/platforms/xcb/proto/xfixes.hpp"
    "include/cppual/ui/platforms/xcb/proto/xinerama.hpp"
    "include/cppual/ui/platforms/xcb/proto/xinput.hpp"
    "include/cppual/ui/platforms/xcb/proto/xprint.hpp"
    "include/cppual/ui/platforms/xcb/proto/xselinux.hpp"
    "include/cppual/ui/platforms/xcb/proto/xtest.hpp"
    "include/cppual/ui/platforms/xcb/proto/xv.hpp"
    "include/cppual/ui/platforms/xcb/proto/xvmc.hpp"
    "include/cppual/ui/platforms/xcb/valueparam.hpp"
    "include/cppual/ui/platforms/xcb/window.hpp"
    "include/cppual/ui/platforms/xcb/xcb.hpp"
    "include/cppual/ui/platforms/xcb/xcb_def.h"
    "include/cppual/ui/platforms/xcb/xcb_iterator.h"
    "src/ui/platform/xorg/xcb_event.h"
    "src/ui/platform/xorg/xbackend.h"
    "src/ui/platform/xorg/xwindow.h"
    "src/ui/platform/xorg/xinput.h"
    "src/ui/platform/xorg/xcb_window.h"
    )

add_library(cppual-ui-plugin-xorg MODULE
    "include/cppual/ui/platforms/xcb/proto/xkb.hpp"
    "src/ui/platform/xorg/xmain.cpp"
    "src/ui/platform/xorg/xbackend.cpp"
    "src/ui/platform/xorg/xwindow.cpp"
    "src/ui/platform/xorg/xinput.cpp"
    )
target_precompile_headers(cppual-ui-plugin-xorg PRIVATE ${CPPUAL_UI_XORG_HEADERS})

endif()

if (WM_FOUND)

# graphical user interface - platform Wayland
set(CPPUAL_UI_WL_HEADERS
    "src/ui/platform/wayland/wlbackend.h"
    "src/ui/platform/wayland/wlsurface.h"
    "src/ui/platform/wayland/wlinput.h"
    )

add_library(cppual-ui-plugin-wayland MODULE
    "src/ui/platform/wayland/wlbackend.cpp"
    "src/ui/platform/wayland/wlsurface.cpp"
    "src/ui/platform/wayland/wlinput.cpp"
    "src/ui/platform/wayland/wlmain.cpp"
    )
target_precompile_headers(cppual-ui-plugin-wayland PRIVATE ${CPPUAL_UI_WL_HEADERS})

endif()

elseif(TARGET cppual-ui AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# graphical user interface - platform Windows
set(CPPUAL_UI_WIN_HEADERS
    "src/ui/platform/winapi/w32window.h"
    "src/ui/platform/winapi/w32backend.h"
    "src/ui/platform/winapi/w32input.h"
    )

add_library(cppual-ui-plugin-win MODULE
    "src/ui/platform/winapi/w32window.cpp"
    "src/ui/platform/winapi/w32backend.cpp"
    "src/ui/platform/winapi/w32input.cpp"
    "src/ui/platform/winapi/w32main.cpp"
    )
target_precompile_headers(cppual-ui-plugin-win PRIVATE ${CPPUAL_UI_WIN_HEADERS})

endif()


### link targets
if(TARGET cppual-memory-system)
    target_link_libraries(cppual-memory-system cppual-endoskeleton)
endif()

if(TARGET cppual-gfx)
    target_link_libraries(cppual-gfx cppual-endoskeleton)

    if(FREETYPE_FOUND)
        target_link_libraries(cppual-gfx-font cppual-gfx cppual-endoskeleton Freetype::Library)
    endif()

    if(OPENGL_FOUND AND GLEW_FOUND)
        target_link_libraries(cppual-gl cppual-gfx cppual-endoskeleton OpenGL::Library GLEW::Library)
    endif()

    if(TARGET cppual-gl AND EGL_FOUND)
        target_link_libraries(cppual-gfx-plugin-egl cppual-gfx cppual-gl OpenGL::Library EGL::Library GLEW::Library)
    endif()

    if(TARGET cppual-gl AND XCB_GLX_FOUND)
        target_link_libraries(cppual-gfx-plugin-glx cppual-gfx cppual-gl OpenGL::Library X11::Library GLEW::Library)
    endif()

    if(VULKAN_FOUND)
        target_link_libraries(cppual-gfx-plugin-vulkan cppual-gfx Vulkan::Library)
    endif()

    target_link_libraries(cppual-ui cppual-gfx cppual-endoskeleton)
    target_link_libraries(cppual-ui-views cppual-ui cppual-endoskeleton)
    target_link_libraries(cppual-ui-dialogs cppual-ui-views cppual-ui)
endif()

if(TARGET cppual-ui AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if (XCB_EWMH_FOUND AND XCB_ICCCM_FOUND AND X11_FOUND)
        target_link_libraries(cppual-ui-plugin-xorg cppual-endoskeleton XCB_EWMH::Library XCB_ICCCM::Library X11::Library)
    elseif (NOT WM_FOUND)
        target_link_libraries(cppual-ui-plugin-wayland cppual-endoskeleton WM::Library)
    endif()
elseif(TARGET cppual-ui AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(cppual-ui-plugin-win cppual-endoskeleton cppual-ui WM::Library)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
#    target_link_libraries(cppual-ui-android cppual-endoskeleton WM::Library)
endif()

if(SOCKETS_FOUND)

target_link_libraries(cppual-endoskeleton DL::Library RT::Library Sockets::Library)
target_link_libraries(cppual-network cppual-endoskeleton Sockets::Library)

endif()


target_link_libraries(cppual-dom cppual-endoskeleton)
target_link_libraries(cppual-json cppual-endoskeleton)

if(TARGET cppual-endoskeleton)
    target_link_libraries(cppual-video-plugin-compute cppual-endoskeleton)
endif()

if(TARGET cppual-endoskeleton AND OPENCL_FOUND)
    target_link_libraries(cppual-compute-plugin-opencl cppual-endoskeleton OpenCL::Library)
endif()

if(TARGET cppual-endoskeleton AND VULKAN_FOUND)
    target_link_libraries(cppual-compute-plugin-vulkan cppual-endoskeleton Vulkan::Library)
endif()


if(SNDFILE_FOUND)
    target_link_libraries(cppual-audio cppual-endoskeleton SndFile::Library)
endif()

#if(TARGET cppual-audio AND OPENAL_FOUND)
if(OPENAL_FOUND)
#    target_link_libraries(cppual-audio-plugin-openal cppual-audio OpenAL::Library)
    target_link_libraries(cppual-audio-plugin-openal
    OpenAL::Library)
endif()


if(TARGET cppual-audio AND TARGET cppual-endoskeleton)

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND PulseAudio_FOUND)
        target_link_libraries(cppual-audio-plugin-compute cppual-audio cppual-endoskeleton PulseAudio::Library)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(cppual-audio-plugin-compute cppual-audio cppual-endoskeleton)
    endif()

endif()

#Cxxdroid-binary: /storage/emulated/0/Documents/Cxxdroid/cppual/bin/Linux-Clang-armv7l/cppual-memory-test

### testing
#enable_testing()

add_executable(cppual-memory-test "tests/memory_test.cpp")

target_link_libraries(cppual-memory-test cppual-endoskeleton)

#add_test (NAME memory_test COMMAND cppual-memory-test)

#add_test(memory_test ${CMAKE_CTEST_COMMAND}
#   --build-and-test "${CMAKE_SOURCE_DIR}/Tests/Simple"
#                    "${CMAKE_BINARY_DIR}/Tests/Simple"
#   --build-generator ${CMAKE_GENERATOR}
#   --build-makeprogram ${CMAKE_MAKE_PROGRAM}
#   --build-project Simple
#   --test-command memory_test)