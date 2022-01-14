cmake_minimum_required(VERSION 3.6)

if(ANDROID)
    #if(NOT DEFINED ANDROID_NDK)
    #    set(ANDROID_NDK "/opt/android-ndk")
    #    set(ANDROID_NDK_ROOT "/opt/android-ndk")
    #endif()
    #set(ANDROID_SDK_ROOT "/opt/android-sdk")
    if(NOT DEFINED ANDROID_ABI)
        set(ANDROID_ABI arm64-v8a)
    endif()
    if(NOT DEFINED ANDROID_PLATFORM)
        set(ANDROID_PLATFORM "android-21")
    endif()
    #if(NOT DEFINED ANDROID_NATIVE_API_LEVEL)
    #    set(ANDROID_NATIVE_API_LEVEL 21)
    #endif()
    if(NOT DEFINED ANDROID_TOOLCHAIN_MACHINE_NAME)
        set(ANDROID_TOOLCHAIN_MACHINE_NAME "arm-linux-androideabi")
    endif()
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

project("C++ UAL" VERSION 1.0.5 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

set(LIBRARY_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(CompilerConfig REQUIRED)

find_package(DL            REQUIRED) ## dynamic loading of libraries
find_package(WM            REQUIRED) ## platform window manager
find_package(XCB COMPONENTS XCB ICCCM EWMH) ## platform window manager (Xcb)
find_package(X11                   ) ## platform window manager (X11-xcb)
find_package(Sockets       REQUIRED) ## network interface & interprocess
find_package(OpenAL                ) ## will be dropped at some point
find_package(SndFile               ) ## will be dropped at some point
find_package(Freetype              ) ## will be dropped at some point
find_package(GLEW                  ) ## will be dropped at some point
find_package(OpenGL        REQUIRED) ## will be dropped at some point
find_package(EGL                   ) ## will be dropped at some point
find_package(OpenCL                ) ## backup compute api
find_package(Vulkan                ) ## primary compute & graphics api

include_directories(
    include
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


### compile libraries

# process, modules
add_library(cppual-host SHARED
    "include/cppual/functional.h"
    "include/cppual/tuple.h"
    "include/cppual/tuple_iterator.h"
    "include/cppual/bitfield.h"
    "include/cppual/coroutines/coroutines.h"
    "include/cppual/fibers/fibers.h"
    "include/cppual/unbound_matrix.h"
    "include/cppual/flags.h"
    "include/cppual/signal.h"
    "include/cppual/object.h"
    "include/cppual/types.h"
    "include/cppual/error.h"
    "include/cppual/common.h"
    "include/cppual/string.h"
    "include/cppual/string_helper.h"
    "include/cppual/math.h"
    "include/cppual/reactive.h"
    "include/cppual/simd.h"
    "include/cppual/meta.h"
    "include/cppual/concepts.h"
    "include/cppual/iterator.h"
    "include/cppual/circular_queue.h"
    "include/cppual/decl.h"
    "include/cppual/types.h"
    "include/cppual/timeline.h"
    "include/cppual/noncopyable.h"
    "src/process/proc_model.cpp"
    "include/cppual/process/proc_model.h"
    "src/process/interproc.cpp"
    "include/cppual/process/interproc.h"
    "include/cppual/process/details.h"
    "include/cppual/process/pipe.h"
    "src/process/plugin.cpp"
    "include/cppual/process/plugin.h"
    "src/system/sysinfo.cpp"
    "include/cppual/system/sysinfo.h"
    "include/cppual/system/clock.h"
    "include/cppual/resource.h"
    )
target_compile_definitions(cppual-host PRIVATE -DAPI_EXPORT)

# memory management
add_library(cppual-memory SHARED
    "include/cppual/memory/system.h"
    "src/memory/system.cpp"
    "include/cppual/memory/static.h"
    "include/cppual/memory/mop.h"
    "src/memory/stack.cpp"
    "include/cppual/memory/stack.h"
    "src/memory/pool.cpp"
    "include/cppual/memory/pool.h"
    "src/memory/heap.cpp"
    "include/cppual/memory/heap.h"
    "src/memory/shared.cpp"
    "include/cppual/memory/page.h"
    "src/memory/page.cpp"
    "include/cppual/memory/shared.h"
    "include/cppual/memory/allocator.h"
    )
target_compile_definitions(cppual-memory PRIVATE -DAPI_EXPORT)

# device compute, memory (Vulkan, OpenCL, Serial)
add_library(cppual-compute SHARED
    "include/cppual/compute/bridge.h"
    "src/compute/bridge.cpp"
    "include/cppual/compute/behaviour.h"
    "src/compute/behaviour.cpp"
    "include/cppual/compute/object.h"
    "src/compute/object.cpp"
    "include/cppual/compute/backend_iface.h"
    "src/compute/backend_iface.cpp"
    "include/cppual/compute/platform.h"
    "include/cppual/compute/program.h"
    "src/compute/program.cpp"
    "include/cppual/compute/device.h"
    "src/compute/device.cpp"
    "include/cppual/compute/devmemory.h"
    "src/compute/devmemory.cpp"
    "include/cppual/compute/devtask.h"
    "src/compute/devtask.cpp"
    "include/cppual/compute/pll_ops.h"
    "src/compute/task.cpp"
    "include/cppual/compute/task.h"
    "src/compute/thread.cpp"
    "include/cppual/compute/thread.h"
    )
target_compile_definitions(cppual-compute PRIVATE -DAPI_EXPORT)

# json
add_library(cppual-json SHARED
    "include/cppual/json/error/en.h"
    "include/cppual/json/error/error.h"
    "include/cppual/json/msinttypes/inttypes.h"
    "include/cppual/json/msinttypes/stdint.h"
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
    "src/json/json_adaptor.cpp"
    )
target_compile_definitions(cppual-json PRIVATE -DAPI_EXPORT)

if(VULKAN_FOUND)

add_library(cppual-compute-vulkan MODULE
    "src/compute/backend/vulkan.h"
    "src/compute/backend/vulkan.cpp"
    )

endif()

if(OPENCL_FOUND)

add_library(cppual-compute-opencl MODULE
    "src/compute/backend/opencl.h"
    "src/compute/backend/opencl.cpp"
    )

endif()

#add_library(cppual-db SHARED
#	"include/cppual/db/model.h"
#	)

# Document Object Model (xml, cmdargs, web)
add_library(cppual-dom SHARED
    "include/cppual/dom/cmdline.h"
    "include/cppual/dom/csv.h"
    "src/dom/csv.cpp"
    "include/cppual/dom/exceptions.h"
    "include/cppual/dom/id_translator.h"
    "include/cppual/dom/ini_parser.h"
    "include/cppual/dom/json_parser.h"
    "include/cppual/dom/ptree_fwd.h"
    "include/cppual/dom/ptree_serialization.h"
    "include/cppual/dom/ptree.h"
    "include/cppual/dom/stream_translator.h"
    "include/cppual/dom/string_path.h"
    "include/cppual/dom/xml_parser.h"

    "include/cppual/dom/json_parser/error.h"
    "include/cppual/dom/json_parser/detail/narrow_encoding.h"
    "include/cppual/dom/json_parser/detail/parser.h"
    "include/cppual/dom/json_parser/detail/read.h"
    "include/cppual/dom/json_parser/detail/standard_callbacks.h"
    "include/cppual/dom/json_parser/detail/wide_encoding.h"
    "include/cppual/dom/json_parser/detail/write.h"

    "include/cppual/dom/detail/exception_implementation.h"
    "include/cppual/dom/detail/file_parser_error.h"
    "include/cppual/dom/detail/ptree_implementation.h"
    "include/cppual/dom/detail/ptree_utils.h"
    "include/cppual/dom/detail/rapidxml.h"
    "include/cppual/dom/detail/xml_parser_error.h"
    "include/cppual/dom/detail/xml_parser_flags.h"
    "include/cppual/dom/detail/xml_parser_read_rapidxml.h"
    "include/cppual/dom/detail/xml_parser_utils.h"
    "include/cppual/dom/detail/xml_parser_write.h"
    "include/cppual/dom/detail/xml_parser_writer_settings.h"
    )
target_compile_definitions(cppual-dom PRIVATE -DAPI_EXPORT)

# transparent network facilities
add_library(cppual-network SHARED
    "src/network/address.cpp"
    "include/cppual/network/address.h"
    "src/network/packet.cpp"
    "include/cppual/network/packet.h"
    "src/network/uri.cpp"
    "include/cppual/network/uri.h"
    "include/cppual/network/uri_parts.h"
    "src/network/transport/socket.cpp"
    "include/cppual/network/transport/socket.h"
    "src/network/transport/tcp.cpp"
    "include/cppual/network/transport/tcp.h"
    "src/network/transport/tcpclient.cpp"
    "include/cppual/network/transport/tcpclient.h"
    "src/network/transport/tcplistener.cpp"
    "include/cppual/network/transport/tcplistener.h"
    "src/network/transport/udp.cpp"
    "include/cppual/network/transport/udp.h"
    "src/network/transport/proxy.cpp"
    "include/cppual/network/transport/proxy.h"
    "src/network/protocols/ftp.cpp"
    "include/cppual/network/protocols/ftp.h"
    "src/network/protocols/http.cpp"
    "include/cppual/network/protocols/http.h"
    "src/network/protocols/p2p.cpp"
    "include/cppual/network/protocols/p2p.h"
    "src/network/protocols/metalink.cpp"
    "include/cppual/network/protocols/metalink.h"
    "src/network/protocols/ssh.cpp"
    "include/cppual/network/protocols/ssh.h"
    "src/network/protocols/protocol.cpp"
    "include/cppual/network/protocols/protocol.h"
    )
target_compile_definitions(cppual-network PRIVATE -DAPI_EXPORT)

# raw device input
add_library(cppual-input SHARED
    "include/cppual/input/event.h"
    "src/input/keyboard.cpp"
    "include/cppual/input/keyboard.h"
    "src/input/pointer.cpp"
    "include/cppual/input/pointer.h"
    "src/input/joystick.cpp"
    "include/cppual/input/joystick.h"
    "include/cppual/input/joysticks/joy_generic.h"
    "include/cppual/input/joysticks/joy_ps.h"
    "include/cppual/input/joysticks/joy_steam.h"
    "include/cppual/input/joysticks/joy_xbox.h"
    "include/cppual/input/joysticks/joy_wheel.h"
    )
target_compile_definitions(cppual-input PRIVATE -DAPI_EXPORT)

# mqtt client implementation
add_library(cppual-mqtt SHARED
    "include/cppual/mqtt/client.h"
    "src/mqtt/client.cpp"
    "include/cppual/mqtt/message.h"
    "src/mqtt/message.cpp"
    )
target_compile_definitions(cppual-mqtt PRIVATE -DAPI_EXPORT)

if(SNDFILE_FOUND AND OPENAL_FOUND)

# audio library (OpenAL, TrueAudio, Compute)
add_library(cppual-audio SHARED
    "src/multimedia/audio/sound.cpp"
    "include/cppual/multimedia/audio/sound.h"
    "src/multimedia/audio/spatial.cpp"
    "include/cppual/multimedia/audio/spatial.h"
    "src/multimedia/audio/format.cpp"
    "include/cppual/multimedia/audio/format.h"
    "src/multimedia/audio/device.cpp"
    "include/cppual/multimedia/audio/device.h"
    "src/multimedia/audio/buffer.cpp"
    "include/cppual/multimedia/audio/buffer.h"
    "src/multimedia/audio/source.cpp"
    "include/cppual/multimedia/audio/source.h"
    "src/multimedia/audio/emitter.cpp"
    "include/cppual/multimedia/audio/emitter.h"
    "src/multimedia/audio/playback.cpp"
    "include/cppual/multimedia/audio/playback.h"
    "src/multimedia/audio/listener.cpp"
    "include/cppual/multimedia/audio/listener.h"
    "src/multimedia/audio/record.cpp"
    "include/cppual/multimedia/audio/record.h"
    )
target_compile_definitions(cppual-audio PRIVATE -DAPI_EXPORT)

# audio backend OpenAL
add_library(cppual-audio-openal MODULE
    "src/multimedia/audio/al/aldef.h"
    "src/multimedia/audio/al/alobject.cpp"
    "include/cppual/multimedia/audio/al/alobject.h"
    "src/multimedia/audio/al/aldevice.cpp"
    "include/cppual/multimedia/audio/al/aldevice.h"
    "src/multimedia/audio/al/albuffer.cpp"
    "include/cppual/multimedia/audio/al/albuffer.h"
    "src/multimedia/audio/al/alsource.cpp"
    "include/cppual/multimedia/audio/al/alsource.h"
    "src/multimedia/audio/al/emitter.cpp"
    "include/cppual/multimedia/audio/al/emitter.h"
    "src/multimedia/audio/al/music.cpp"
    "include/cppual/multimedia/audio/al/music.h"
    "src/multimedia/audio/al/listener.cpp"
    "include/cppual/multimedia/audio/al/listener.h"
    "src/multimedia/audio/al/recorder.cpp"
    "include/cppual/multimedia/audio/al/recorder.h"
    )

endif()

# multimedia
add_library(cppual-multimedia SHARED
    "src/multimedia/media_mgr.cpp"
    "include/cppual/multimedia/media_mgr.h"
    "src/multimedia/media_sync.cpp"
    "include/cppual/multimedia/media_sync.h"
    )
target_compile_definitions(cppual-multimedia PRIVATE -DAPI_EXPORT)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# video acceleration api backend
add_library(cppual-video-vaapi MODULE
    "src/multimedia/video/backend/vaapi.cpp"
    )

# Xorg video backend
add_library(cppual-video-xorg MODULE
    "src/multimedia/video/backend/xvideo.cpp"
    )

endif()

# compute video backend
add_library(cppual-video-compute MODULE
    "src/multimedia/video/backend/compute_video.cpp"
    )

# video library (VAAPI, XVideo, OpenGL)
add_library(cppual-video SHARED
    "src/multimedia/video/vfilter.cpp"
    "include/cppual/multimedia/video/vfilter.h"
    "src/multimedia/video/vequalizer.cpp"
    "include/cppual/multimedia/video/vequalizer.h"
    )
target_compile_definitions(cppual-video PRIVATE -DAPI_EXPORT)

if(FREETYPE_FOUND AND OPENGL_FOUND AND GLEW_FOUND AND EGL_FOUND)

# graphics drawing library
add_library(cppual-gfx SHARED
    "include/cppual/gfx/coord.h"
    "src/gfx/color.cpp"
    "include/cppual/gfx/color.h"
    "src/gfx/matrix.cpp"
    "include/cppual/gfx/matrix.h"
    "src/gfx/draw.cpp"
    "include/cppual/gfx/draw.h"
    "src/gfx/font.cpp"
    "include/cppual/gfx/font.h"
    "src/gfx/formats/bmp.cpp"
    "include/cppual/gfx/formats/bmp.h"
    )
target_compile_definitions(cppual-gfx PRIVATE -DAPI_EXPORT)

# OpenGL/ES library
add_library(cppual-gl SHARED
    "src/gfx/gl/gldef.h"
    "src/gfx/gl/glbase.cpp"
    "include/cppual/gfx/gl/glbase.h"
    "src/gfx/gl/runtime.cpp"
    "include/cppual/gfx/gl/runtime.h"
    "src/gfx/gl/surface.cpp"
    "include/cppual/gfx/gl/surface.h"
    "src/gfx/gl/glsl.cpp"
    "include/cppual/gfx/gl/glsl.h"
    "src/gfx/gl/texture.cpp"
    "include/cppual/gfx/gl/texture.h"
    "src/gfx/gl/buffer.cpp"
    "include/cppual/gfx/gl/buffer.h"
    "src/gfx/gl/vbo.cpp"
    "include/cppual/gfx/gl/vbo.h"
    "src/gfx/gl/framebuf.cpp"
    "include/cppual/gfx/gl/framebuf.h"
    "src/gfx/gl/query.cpp"
    "include/cppual/gfx/gl/query.h"
    "src/gfx/gl/batch.cpp"
    "include/cppual/gfx/gl/batch.h"
    "src/gfx/gl/instance.cpp"
    "include/cppual/gfx/gl/instance.h"
    )
target_compile_definitions(cppual-gl PRIVATE -DAPI_EXPORT)

# gfx formats library
add_library(cppual-gfx-formats MODULE
    "include/cppual/gfx/formats/bmp.h"
    "src/gfx/formats/bmp.cpp"
    )

# drawable objects library
add_library(cppual-gfx-drawables MODULE
    "include/cppual/gfx/drawable/shape.h"
    "src/gfx/drawable/image.cpp"
    "include/cppual/gfx/drawable/image.h"
    "src/gfx/drawable/text.cpp"
    "include/cppual/gfx/drawable/text.h"
    "src/gfx/drawable/elipse.cpp"
    "include/cppual/gfx/drawable/elipse.h"
    "src/gfx/drawable/rectangle.cpp"
    "include/cppual/gfx/drawable/rectangle.h"
    "src/gfx/drawable/polygon.cpp"
    "include/cppual/gfx/drawable/polygon.h"
    "src/gfx/drawable/line.cpp"
    "include/cppual/gfx/drawable/line.h"
    )

# graphical user interface
add_library(cppual-ui SHARED
    "src/ui/manager.cpp"
    "include/cppual/ui/manager.h"
    "src/ui/display.cpp"
    "include/cppual/ui/display.h"
    "src/ui/queue.cpp"
    "include/cppual/ui/queue.h"
    "include/cppual/ui/wm.h"
    "src/ui/window.cpp"
    "include/cppual/ui/window.h"
    "src/ui/vsurface.cpp"
    "include/cppual/ui/vsurface.h"
    "src/ui/animation.cpp"
    "include/cppual/ui/animation.h"
    "src/ui/composite.cpp"
    "include/cppual/ui/composite.h"
    "src/ui/view.cpp"
    "include/cppual/ui/view.h"
    "src/ui/skin.cpp"
    "include/cppual/ui/skin.h"
    "src/ui/layout.cpp"
    "include/cppual/ui/layout.h"
    "src/ui/hotkey.cpp"
    "include/cppual/ui/hotkey.h"
    #"src/ui/cmd.cpp""
    "include/cppual/ui/cmd.h"
    #"src/ui/groups.cpp"
    "include/cppual/ui/groups.h"
    )
target_compile_definitions(cppual-ui PRIVATE -DAPI_EXPORT)

# graphical user interface - windows and controls
add_library(cppual-ui-views SHARED
    "src/ui/frame.cpp"
    "include/cppual/ui/frame.h"
    "src/ui/dialog.cpp"
    "include/cppual/ui/dialog.h"
    "src/ui/controls/label.cpp"
    "include/cppual/ui/controls/label.h"
    "src/ui/controls/button.cpp"
    "include/cppual/ui/controls/button.h"
    "src/ui/controls/popup.cpp"
    "include/cppual/ui/controls/popup.h"
    )
target_compile_definitions(cppual-ui-views PRIVATE -DAPI_EXPORT)

# graphical user interface - native dialogs
add_library(cppual-ui-dialogs MODULE
    "src/ui/dialogs/msgdlg.cpp"
    "include/cppual/ui/dialogs/msgdlg.h"
    "include/cppual/ui/dialogs/colordlg.h"
    "include/cppual/ui/dialogs/filedlg.h"
    "include/cppual/ui/dialogs/printdlg.h"
    "include/cppual/ui/dialogs/taskdlg.h"
    "include/cppual/ui/dialogs/wizarddlg.h"
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# graphical user interface - platform Xorg
add_library(cppual-ui-xorg MODULE
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
    "include/cppual/ui/platforms/xcb/proto/xkb.hpp"
    "include/cppual/ui/platforms/xcb/proto/xprint.hpp"
    "include/cppual/ui/platforms/xcb/proto/xselinux.hpp"
    "include/cppual/ui/platforms/xcb/proto/xtest.hpp"
    "include/cppual/ui/platforms/xcb/proto/xv.hpp"
    "include/cppual/ui/platforms/xcb/proto/xvmc.hpp"
    "include/cppual/ui/platforms/xcb/valueparam.hpp"
    "include/cppual/ui/platforms/xcb/window.hpp"
    "include/cppual/ui/platforms/xcb/xcb.hpp"
    "src/ui/platform/xorg/xmain.cpp"
    "src/ui/platform/xorg/xbackend.cpp"
    "src/ui/platform/xorg/xbackend.h"
    "src/ui/platform/xorg/xsurface.cpp"
    "src/ui/platform/xorg/xsurface.h"
    "src/ui/platform/xorg/xinput.cpp"
    "src/ui/platform/xorg/xinput.h"
    "src/ui/platform/xorg/xcb_def.h"
    "src/ui/platform/xorg/xcb_event.h"
    "src/ui/platform/xorg/xcb_window.h"
    "src/ui/platform/xorg/xcb_iterator.h"
    )

# graphical user interface - platform Wayland
add_library(cppual-ui-wayland MODULE
    "src/ui/platform/wayland/wlbackend.cpp"
    "src/ui/platform/wayland/wlbackend.h"
    "src/ui/platform/wayland/wlsurface.cpp"
    "src/ui/platform/wayland/wlsurface.h"
    "src/ui/platform/wayland/wlinput.cpp"
    "src/ui/platform/wayland/wlinput.h"
    "src/ui/platform/wayland/wlmain.cpp"
    )

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# graphical user interface - platform Windows
add_library(cppual-ui-win MODULE
    "src/ui/platform/winapi/w32window.cpp"
    "src/ui/platform/winapi/w32window.h"
    "src/ui/platform/winapi/w32backend.cpp"
    "src/ui/platform/winapi/w32backend.h"
    "src/ui/platform/winapi/w32input.cpp"
    "src/ui/platform/winapi/w32input.h"
    "src/ui/platform/winapi/w32main.cpp"
    )

endif()

endif()


### link targets
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(cppual-ui-xorg cppual-memory XCB_EWMH::Library XCB_ICCCM::Library X11::Library)
    target_link_libraries(cppual-ui-wayland cppual-memory WM::Library)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows" AND GLEW_FOUND AND EGL_FOUND)
    target_link_libraries(cppual-ui-win cppual-memory WM::Library)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
#    target_link_libraries(cppual-memory)
endif()

target_link_libraries(cppual-host DL::Library)
target_link_libraries(cppual-compute cppual-memory cppual-host Sockets::Library)
target_link_libraries(cppual-network cppual-memory Sockets::Library)
target_link_libraries(cppual-dom cppual-memory)

target_link_libraries(cppual-video-compute cppual-compute)

if(OPENCL_FOUND)
    target_link_libraries(cppual-compute-opencl cppual-compute OpenCL::Library)
endif()

if(VULKAN_FOUND)
    target_link_libraries(cppual-compute-vulkan cppual-compute Vulkan::Library)
endif()

if(SNDFILE_FOUND AND OPENAL_FOUND)
    target_link_libraries(cppual-audio cppual-compute SndFile::Library)
    target_link_libraries(cppual-audio-openal cppual-audio OpenAL::Library)
endif()

if(FREETYPE_FOUND AND OPENGL_FOUND AND GLEW_FOUND AND EGL_FOUND)
    target_link_libraries(cppual-gfx cppual-compute Freetype::Library)
    target_link_libraries(cppual-gfx-drawables cppual-gl cppual-gfx)
    target_link_libraries(cppual-gl cppual-gfx EGL::Library GLEW::Library OpenGL::Library)
    target_link_libraries(cppual-ui cppual-gfx)
    target_link_libraries(cppual-ui-views cppual-ui)
    target_link_libraries(cppual-ui-dialogs cppual-ui-views cppual-ui)
endif()
