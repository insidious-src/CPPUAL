# C++ Unified Abstraction Library #

**Latest Supported Standard: C++14**

### Why reinvent the wheel and replace the standard library and Qt? ###
- The whole idea of the library is to implement everything as a host and compute devices for parallelism, deprecating OpenGL, DirectX, VA, VDPAU, OVD, TrueAudio and OpenAL completely.

So the real question is: Why would you want to have all these libraries when OpenCL is pushing hard and you can program it pretty much like a regular CPU? It's just a matter of time until OpenCL turns into a all-in-one standard.


### Core ###
- cross-platform process management and interprocess communication
- unified runtime allocator management for the host and all devices

### Compute ###
- use concurrency as host implementation
- unified out-of-order tasks and continuation tasks
- use OpenCL as a SINGLE SOURCE backend (SPIR is required to implement the C++ interpreter)
- behaviour-based device scheduling and context manipulation
- branched device tasks

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
- circular buffer object
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
- fast string
- circular queue + atomic
- signals
- reactive functional types
- simd types
- resource management

### System ###
- query hardware and os information
- timer
- calendar