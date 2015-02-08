![cppual.png](https://bitbucket.org/repo/jXXjaG/images/1113278086-cppual.png)
# C++ Unified Abstraction Library #

**Latest Supported Language Standard: C++14**

### Why reinvent the wheel and replace the standard library and Qt? ###
- the whole idea is to implement everything as a host and compute devices for parallelism, deprecating OpenGL, DirectX, VA, VDPAU, OVD, XVideo, TrueAudio and OpenAL completely.
- behaviour-based device switching and events

**So the real question is:** Why would you want to have all these libraries when OpenCL is pushing hard and you can program it pretty much like a regular CPU? It's just a matter of time until OpenCL turns into a all-in-one standard.


### Core ###
- cross-platform process management and interprocess communication
- unified runtime allocator management for the host and all devices

### Compute ###
- use concurrency as host implementation
- host/device out-of-order and continuation tasks
- use OpenCL as a SINGLE SOURCE backend (SPIR is required to implement the C++ interpreter)
- behaviour-based device scheduling and context manipulation
- branched device tasks

### Input ###
- specialized raw input queues
- integrated security

### Network ###
- implementation of the C++17 network library
- simplified packed exchange
- advanced protocol interface
- http, https, ftp protocols

### Graphics ###
- unified surface management using EGL
- gfx algorithms based on compute library
- abstract draw objects
- scene graph
- 2D images and icons
- 3D images
- cross-platform font texture generator
- color management and post-processing

### User Interface ###
- display management
- window adaptor for platform window management
- completely independant widgets implemented as functional structures (can be used everywhere from 3D environments to desktop)
- abstract controls (draw objects can be changed on the fly)
- skin engine (native, html/css, xml & custom)

### Multimedia ###
- codecs backend manager
- audio/video synchronization and manipulation

### Audio ###
- audio backend using compute library
- audio file loader (format decoder, quality convertion and manipulation)
- listener functions
- sound source
- dynamic circular buffer
- emitter
- advanced player
- equalizer and filters

### Video ###
- video backend using compute library
- equalizer and filters
- multi-screen support
- desktop recording

### Utility ###
- passive timeline with speed adjustment
- fast COW string
- circular queue + atomic
- signals
- reactive functional types
- simd types
- resource management

### System ###
- query hardware and os information
- timer
- calendar

### Longterm Goals ###
- drop freetype
- drop opengl, directx, glew
- drop openal
- drop sndfile
- transparent filesystem managament
- prepare the library for universal memory (memristors)
- interprocess compute runtime to balance device resources in the whole system
- implement everything in D Language