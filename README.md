# C++ Unified Abstraction Library #

## Latest Supported Standard: C++14 ##

### Core: ###
- cross-platform process management and interprocess communication
- unified runtime allocator management

### Compute: ###
- use concurrency as host implementation
- unified out-of-order tasks and continuation tasks
- use OpenCL as a SINGLE SOURCE backend (SPIR is required to implement the C++ interpreter)
- behaviour based device scheduling and context manipulation
- branched device tasks

### Graphics: ###
- unified surface management using EGL
- gfx algorithms based on compute library
- abstract draw objects
- scene graph
- 2D images and icons
- 3D images
- cross-platform font texture generator
- color management and post-processing

User Interface:
- display management
- window adaptor for platform window management
- completely independant widgets implemented as functional structures (can be used everywhere from 3D environments to desktop)
- abstract controls (draw objects can be changed on the fly)
- skin engine (native, html/css, xml & custom)

### Multimedia: ###
- codecs backend manager
- audio/video synchronization and manipulation

Audio:
- audio backend using compute library
- audio file loader (format decoder, quality convertion and manipulation)
- listener functions
- sound source
- circular buffer object
- emitter
- advanced player
- equalizer and filters

### Video: ###
- video backend using compute library
- equalizer and filters
- multi-screen support
- desktop recording

### System: ###
- query hardware and os information
- timer
- calendar