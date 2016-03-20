![Logo](https://bitbucket.org/repo/jXXjaG/images/1113278086-cppual.png "C++ Unified Abstraction Library")

# C++ Unified Abstraction Library #

	Latest Supported Language Standard: C++14
	Future Language Standards: C++17 or D Language

### Why reinvent the wheel and replace parts of the standard library and Qt? ###
- the whole idea is to implement everything as a host and compute devices for parallelism, deprecating OpenGL, DirectX, VA, VDPAU, OVD, XVideo, TrueAudio and OpenAL completely.
- behaviour-based device processing and explicit events

**So the real question is:** Why would you want to have all these libraries when OpenCL is pushing hard and you can program it pretty much like a regular CPU? It's just a matter of time until OpenCL evolves and turns into an all-in-one standard.

**March 6th 2015:** Well,... it evolved into Vulkan, however OpenCL still remains as a new revision was released (2.1). Both are using a common IL - SPIR-V.


### Host ###
* modules **(Done)**
* utilities **(Done)**
* cross-platform process management and interprocess communication **(~60%)**
* query hardware and os information **(Done)**

### Compute ###
* use concurrency as host implementation **(Done)**
* specialized device & shared memory allocators **(~80%)**
* unified memory management & allocator patterns **(~90%)**
* host/device out-of-order and continuation tasks **(~50%)**
* use Vulkan & OpenCL as SINGLE SOURCE backends (SPIR-V is required to implement eventual C++ interpreter) **(~40%)**
* behaviour-based device scheduling and manipulation **(WIP)**
* branched device tasks **(WIP)**

### Input ###
* specialized raw input queues **(WIP)**
* integrated security
* all types of keyboards
* all types of mice
* generic controller interface & support
* wheel controller
* Xbox 360/One controller
* Playstation 1/2/4 controller
* Steam controller
* extended button layouts
* touch surfaces

### Network ###
* implementation of the C++17 network library **(~80%)**
* simplified packet exchange **(Done)**
* layered protocol interface **(~30%)**
* http, https, ftp protocols

### Graphics ###
* unified surface management using Vulkan Extensions **(WIP)**
* gfx algorithms based on compute library **(WIP)**
* abstract draw objects
* scene graph
* 2D images and icons
* 3D images
* cross-platform font texture generator **(WIP)**
* color management and post-processing

### User Interface ###
* display management (Done)
* window adaptor for platform window management **(~90%)**
* completely independant widgets implemented as functional structures (can be used everywhere from 3D environments to desktop) **(~80%)**
* abstract interchangeable controls (draw objects can be changed on the fly) **(WIP)**
* skin engine (native, html/css, xml & custom) **(WIP)**

### Multimedia ###
* codecs backend manager
* audio/video synchronization and manipulation

### Audio ###
* audio backend using compute library
* audio file loader (format decoder, quality convertion and manipulation) **(~30%)**
* listener functions **(Done)**
* recorder **(Done)**
* sound source **(Done)**
* dynamic circular buffer **(Done)**
* emitter **(Done)**
* advanced player **(WIP)**
* equalizer and filters

### Video ###
* video backend using compute library
* equalizer and filters
* multi-screen support
* desktop recording

### Utility ###
* timer **(Done)**
* calendar
* passive timeline with speed adjustment **(Done)**
* fast COW string **(Done)**
* circular queue + atomic **(WIP)**
* signals **(Done)**
* reactive functional types **(WIP)**
* simd types
* resource management **(WIP)**

### Longterm Goals ###
* completely integrate into the std library **(~40%)**
* unified dynamic memory management for both host and compute devices **(~80%)**
* drop opengl, directx, glew, egl **(WIP)**
* drop freetype **(WIP)**
* drop openal **(WIP)**
* drop sndfile
* transparent filesystem management (using protocols)
* prepare the library for universal memory (memristors)
* implement everything in D Language
