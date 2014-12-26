project("C++ UAL")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)

cmake_minimum_required(VERSION 3.0)

find_package(X11      REQUIRED)
find_package(OpenAL   REQUIRED)
find_package(OpenGL   REQUIRED)
find_package(GLEW     REQUIRED)
find_package(Freetype REQUIRED)

include(cmake/cpp14defs)
aux_source_directory(. SRC_LIST)
include_directories(include ${FREETYPE_INCLUDE_DIRS})


### compile libraries

# process, modules, memory
add_library(cppual-core SHARED
	include/cppual/callable.h
	include/cppual/delegate.h
	include/cppual/flags.h
	include/cppual/signal.h
	include/cppual/object.h
	include/cppual/types.h
	include/cppual/error.h
	include/cppual/common.h
	include/cppual/mstring.h
	include/cppual/reactive.h
	include/cppual/simd.h
	include/cppual/meta.h
	include/cppual/concepts.h
	include/cppual/iterator.h
	include/cppual/circular_queue.h
	include/cppual/decl.h
	include/cppual/types.h
	include/cppual/noncopyable.h
	src/memory/memory.cpp
	include/cppual/memory/memory.h
	include/cppual/memory/allocator.h
	include/cppual/memory/static.h
	include/cppual/memory/mop.h
	src/memory/stack.cpp
	include/cppual/memory/stack.h
	src/memory/pool.cpp
	include/cppual/memory/pool.h
	src/memory/heap.cpp
	include/cppual/memory/heap.h
	src/memory/shared.cpp
	include/cppual/memory/page.h
	src/memory/page.cpp
	include/cppual/memory/shared.h
	src/process/proc_model.cpp
	include/cppual/process/proc_model.h
	src/process/interproc.cpp
	include/cppual/process/interproc.h
	include/cppual/process/details.h
	include/cppual/process/pipe.h
	src/process/module.cpp
	include/cppual/process/module.h
	src/system/sysinfo.cpp
	include/cppual/system/sysinfo.h
	include/cppual/system/clock.h
	)

 # device compute (OpenCL, CUDA, DirectCompute, GLCompute, Serial)
add_library(cppual-compute SHARED
	include/cppual/resource.h
	include/cppual/compute/vector.h
	include/cppual/compute/list.h
	include/cppual/compute/circular.h
	src/compute/cldef.h
	include/cppual/compute/platform.h
	src/compute/opencl.cpp
	include/cppual/compute/clobject.h
	src/compute/clobject.cpp
	include/cppual/compute/program.h
	src/compute/program.cpp
	include/cppual/compute/device.h
	src/compute/device.cpp
	include/cppual/compute/devmemory.h
	include/cppual/compute/plltask.h
	src/compute/plltask.cpp
	src/compute/clmemory.cpp
	include/cppual/compute/clmemory.h
	src/compute/context.cpp
	include/cppual/compute/context.h
	include/cppual/compute/pll_ops.h
	src/compute/task.cpp
	include/cppual/compute/task.h
	)

#add_library(cppual-db SHARED
#	include/cppual/db/model.h
#	)

# Document Object Model (xml, cmdargs, web)
add_library(cppual-dom SHARED
	src/dom/xml.cpp
	include/cppual/dom/xml.h
	include/cppual/dom/cmdline.h
	include/cppual/dom/dom.h
	include/cppual/dom/pdf.h
	include/cppual/dom/doc.h
	)

# Document Object Model (xml, cmdargs, web)
add_library(cppual-stream SHARED
	include/cppual/stream/iobase.h
	include/cppual/stream/filesystem.h
	include/cppual/stream/device_memory.h
	src/stream/device_memory.cpp
	include/cppual/stream/console.h
	)

# transparent network facilities
add_library(cppual-network SHARED
	src/network/address.cpp
	include/cppual/network/address.h
	src/network/packet.cpp
	include/cppual/network/packet.h
	src/network/uri.cpp
	include/cppual/network/uri.h
	src/network/transport/socket.cpp
	include/cppual/network/transport/socket.h
	src/network/transport/tcp.cpp
	include/cppual/network/transport/tcp.h
	src/network/transport/tcpclient.cpp
	include/cppual/network/transport/tcpclient.h
	src/network/transport/tcplistener.cpp
	include/cppual/network/transport/tcplistener.h
	src/network/transport/udp.cpp
	include/cppual/network/transport/udp.h
	)

# raw device input
add_library(cppual-input SHARED
	include/cppual/input/event.h
	src/input/keyboard.cpp
	include/cppual/input/keyboard.h
	src/input/pointer.cpp
	include/cppual/input/pointer.h
	src/input/joystick.cpp
	include/cppual/input/joystick.h
	include/cppual/input/joysticks/joy_generic.h
	include/cppual/input/joysticks/joy_ps.h
	include/cppual/input/joysticks/joy_steam.h
	include/cppual/input/joysticks/joy_xbox.h
	include/cppual/input/joysticks/joy_wheel.h
	)

# graphical user interface - platform Xorg
add_library(cppual-ui-xorg MODULE
	src/ui/platform/xorg/xmain.cpp
	src/ui/platform/xorg/xbackend.cpp
	src/ui/platform/xorg/xbackend.h
	src/ui/platform/xorg/xsurface.cpp
	src/ui/platform/xorg/xsurface.h
	src/ui/platform/xorg/xinput.cpp
	src/ui/platform/xorg/xinput.h
	)

# graphical user interface - platform Wayland
add_library(cppual-ui-wayland MODULE
	src/ui/platform/wayland/wlbackend.cpp
	src/ui/platform/wayland/wlsurface.cpp
	src/ui/platform/wayland/wlinput.cpp
	)

# graphics drawing library
add_library(cppual-gfx SHARED
	include/cppual/gfx/dsp_details.h
	include/cppual/gfx/coord.h
	src/gfx/color.cpp
	include/cppual/gfx/color.h
	src/gfx/matrix.cpp
	include/cppual/gfx/matrix.h
	src/gfx/draw.cpp
	include/cppual/gfx/draw.h
	src/gfx/font.cpp
	include/cppual/gfx/font.h
	)

# OpenGL/ES library
add_library(cppual-gl SHARED
	src/gfx/gl/gldef.h
	src/gfx/gl/glbase.cpp
	include/cppual/gfx/gl/glbase.h
	src/gfx/gl/runtime.cpp
	include/cppual/gfx/gl/runtime.h
	src/gfx/gl/egl.cpp
	include/cppual/gfx/gl/egl.h
	src/gfx/gl/glsl.cpp
	include/cppual/gfx/gl/glsl.h
	src/gfx/gl/texture.cpp
	include/cppual/gfx/gl/texture.h
	src/gfx/gl/buffer.cpp
	include/cppual/gfx/gl/buffer.h
	src/gfx/gl/vbo.cpp
	include/cppual/gfx/gl/vbo.h
	src/gfx/gl/framebuf.cpp
	include/cppual/gfx/gl/framebuf.h
	src/gfx/gl/query.cpp
	include/cppual/gfx/gl/query.h
	src/gfx/gl/batch.cpp
	include/cppual/gfx/gl/batch.h
	src/gfx/gl/instance.cpp
	include/cppual/gfx/gl/instance.h
	)

# OpenGL/ES library
add_library(cppual-gfx-drawables MODULE
	include/cppual/gfx/drawable/shape.h
	src/gfx/drawable/image.cpp
	include/cppual/gfx/drawable/image.h
	src/gfx/drawable/text.cpp
	include/cppual/gfx/drawable/text.h
	src/gfx/drawable/elipse.cpp
	include/cppual/gfx/drawable/elipse.h
	src/gfx/drawable/rectangle.cpp
	include/cppual/gfx/drawable/rectangle.h
	src/gfx/drawable/polygon.cpp
	include/cppual/gfx/drawable/polygon.h
	src/gfx/drawable/line.cpp
	include/cppual/gfx/drawable/line.h
	)

# video library (VAAPI, XVideo, OpenGL)
add_library(cppual-multimedia SHARED
	src/multimedia/media_mgr.cpp
	include/cppual/multimedia/media_mgr.h
	src/multimedia/media_sync.cpp
	include/cppual/multimedia/media_sync.h)

# video acceleration api backend
add_library(cppual-video-vaapi MODULE
	src/multimedia/video/backend/vaapi.cpp
)

# Xorg video backend
add_library(cppual-video-xorg MODULE
	src/multimedia/video/backend/xvideo.cpp
)

# OpenGL video backend
add_library(cppual-video-gl MODULE
	src/multimedia/video/backend/vgl.cpp
)

# OpenGL video backend
add_library(cppual-video-compute MODULE
	src/multimedia/video/backend/compute_video.cpp
)

# video library (VAAPI, XVideo, OpenGL)
add_library(cppual-video SHARED
	src/multimedia/video/vfilter.cpp
	include/cppual/multimedia/video/vfilter.h
	src/multimedia/video/vequalizer.cpp
	include/cppual/multimedia/video/vequalizer.h)

# audio library (OpenAL, TrueAudio)
add_library(cppual-audio SHARED
	src/multimedia/audio/sound.cpp
	include/cppual/multimedia/audio/sound.h
	src/multimedia/audio/spatial.cpp
	include/cppual/multimedia/audio/spatial.h
	src/multimedia/audio/format.cpp
	include/cppual/multimedia/audio/format.h
	src/multimedia/audio/device.cpp
	include/cppual/multimedia/audio/device.h
	src/multimedia/audio/buffer.cpp
	include/cppual/multimedia/audio/buffer.h
	src/multimedia/audio/source.cpp
	include/cppual/multimedia/audio/source.h
	src/multimedia/audio/emitter.cpp
	include/cppual/multimedia/audio/emitter.h
	src/multimedia/audio/playback.cpp
	include/cppual/multimedia/audio/playback.h
	src/multimedia/audio/listener.cpp
	include/cppual/multimedia/audio/listener.h
	src/multimedia/audio/record.cpp
	include/cppual/multimedia/audio/record.h)

# audio backend OpenAL
add_library(cppual-audio-openal MODULE
	src/multimedia/audio/al/alobject.cpp
	include/cppual/multimedia/audio/al/alobject.h
	src/multimedia/audio/al/aldevice.cpp
	include/cppual/multimedia/audio/al/aldevice.h
	src/multimedia/audio/al/albuffer.cpp
	include/cppual/multimedia/audio/al/albuffer.h
	src/multimedia/audio/al/alsource.cpp
	include/cppual/multimedia/audio/al/alsource.h
	src/multimedia/audio/al/emitter.cpp
	include/cppual/multimedia/audio/al/emitter.h
	src/multimedia/audio/al/music.cpp
	include/cppual/multimedia/audio/al/music.h
	src/multimedia/audio/al/listener.cpp
	include/cppual/multimedia/audio/al/listener.h
	src/multimedia/audio/al/recorder.cpp
	include/cppual/multimedia/audio/al/recorder.h
	)

# graphical user interface
add_library(cppual-ui SHARED
	src/ui/manager.cpp
	include/cppual/ui/manager.h
	src/ui/display.cpp
	include/cppual/ui/display.h
	src/ui/events.cpp
	include/cppual/ui/events.h
	include/cppual/ui/wm.h
	src/ui/vsurface.cpp
	include/cppual/ui/vsurface.h
	src/ui/animation.cpp
	include/cppual/ui/animation.h
	include/cppual/ui/default_events.h
	src/ui/composite.cpp
	include/cppual/ui/composite.h
	src/ui/uimgr.cpp
	include/cppual/ui/uimgr.h
	src/ui/view.cpp
	include/cppual/ui/view.h
	src/ui/skin.cpp
	include/cppual/ui/skin.h
	src/ui/layout.cpp
	include/cppual/ui/layout.h
	src/ui/hotkey.cpp
	include/cppual/ui/hotkey.h
	#src/ui/cmd.cpp
	include/cppual/ui/cmd.h
	#src/ui/groups.cpp
	include/cppual/ui/groups.h
	)

# graphical user interface - windows and controls
add_library(cppual-ui-views SHARED
	src/ui/frame.cpp
	include/cppual/ui/frame.h
	src/ui/window.cpp
	include/cppual/ui/window.h
	src/ui/dialog.cpp
	include/cppual/ui/dialog.h
	src/ui/controls/button.cpp
	include/cppual/ui/controls/button.h
	src/ui/controls/popup.cpp
	include/cppual/ui/controls/popup.h
	)

# graphical user interface - native dialogs
add_library(cppual-ui-dialogs MODULE
	src/ui/dialogs/msgdlg.cpp
	include/cppual/ui/dialogs/msgdlg.h
	include/cppual/ui/dialogs/colordlg.h
	include/cppual/ui/dialogs/filedlg.h
	include/cppual/ui/dialogs/printdlg.h
	include/cppual/ui/dialogs/taskdlg.h
	include/cppual/ui/dialogs/wizarddlg.h
	)


### link targets

target_link_libraries(cppual-core dl)
target_link_libraries(cppual-compute cppual-core pthread OpenCL)
#target_link_libraries(cppual-input)
target_link_libraries(cppual-gfx freetype)
target_link_libraries(cppual-gfx-drawables cppual-gl cppual-gfx GLEW GL)
target_link_libraries(cppual-video-vaapi va)
target_link_libraries(cppual-video-gl cppual-gl GLEW GL)
target_link_libraries(cppual-video-compute cppual-compute)
target_link_libraries(cppual-audio cppual-compute sndfile)
target_link_libraries(cppual-audio-openal openal)
target_link_libraries(cppual-gl cppual-gfx EGL GLEW GL)
target_link_libraries(cppual-ui-xorg xcb-icccm)
target_link_libraries(cppual-ui-wayland wayland-client wayland-cursor)
target_link_libraries(cppual-ui cppual-core)
target_link_libraries(cppual-ui-views cppual-ui)
target_link_libraries(cppual-ui-dialogs cppual-ui-views cppual-ui)


### parse files

file(
	GLOB_RECURSE
	SRC_FILES
	${CMAKE_SOURCE_DIR}/src/*
	)

file(
	GLOB_RECURSE
	INCLUDE_FILES
	${CMAKE_SOURCE_DIR}/include/*
	)
