![Logo](https://raw.githubusercontent.com/insidious-src/CPPUAL/master/cppual.png "C++ Unified Abstraction Library")

>Latest Supported Language Standard: C++20

***[The Source Code - Github](https://github.com/insidious-src/CPPUAL)***

## Framework Goals

#### Why reinvent the wheel and replace parts of the standard library and Qt?
- the whole idea is to implement everything as a host and compute devices for parallelism, deprecating OpenGL, DirectX, VA, VDPAU, OVD, XVideo, TrueAudio and OpenAL completely.
- behaviour-based device processing and explicit events

**So the real question is:** Why would you want to have all these libraries when OpenCL is pushing hard and you can program it pretty much like a regular CPU? It's just a matter of time until OpenCL evolves and turns into an all-in-one standard.

**March 6th 2015:** Well,... it evolved into Vulkan (GLNext, based on Mantle), however OpenCL still remains as a new revision was released (2.1). Both are using a common IL - SPIR-V.


## Concept & Interface Goals

- A library imitating low-level brain communication system
- Focus on parallel execution model (and not on the compute backends)
- Constructive self-registering interfaces


## Technical Specifications

### Host
* plugins
* utilities
* cross-platform process management and interprocess communication
* query hardware and os information

### Compute
* use concurrency as fallback host implementation
* specialized device & shared memory allocators
* unified memory management & allocator patterns
* host/device out-of-order and continuation tasks
* use Vulkan & OpenCL as SINGLE SOURCE backends (SPIR-V is required to implement eventual C++ interpreter)
* behaviour-based device scheduling and manipulation
* branched device tasks

### Active Model View Controller (UniFace)
* node based + smart pointer
* the model not only contains data but also logic
* a logic node can be bound to single or multiple data nodes
* cyclic binding (actually the idea is to ABUSE it)
* can generate multiple model repositories
* interprocess interface
* the interface is protocol agnostic (can exchange data across networks)
* controllers are represented as signal structures + configuration data to interact with a model's logic
* constructive plugin interface that self-registers metadata and functions
* the interface knows about all its functionality as if it was predefined
* the whole interdependent pattern can be used to form a universal communication system for almost any type of program
* can generate user interfaces on the fly using layout data

### Network
* implementation of the C++17 network library
* simplified packet exchange
* layered protocol interface
* http, https, ftp, bittorrent protocols

### Universal Database Interface
* based on AMVC
* standard & relational database model extension
* DOM model extension

### Input
* specialized raw input queues
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

### Graphics
* unified surface management (ex. using Vulkan Extensions)
* gfx algorithms based on compute library
* abstract draw objects
* scene graph
* 2D images and icons
* 3D images
* cross-platform font texture generator
* color management and post-processing

### User Interface
* display management
* window adaptor for platform window management
* completely independant widgets implemented as functional structures (can be used everywhere from 3D environments to desktop)
* abstract interchangeable controls (draw objects can be changed on the fly)
* skin engine (native, html/css, xml & custom)

### Multimedia
* codecs backend manager
* audio/video synchronization and manipulation

### Audio
* audio backend using compute library
* audio file loader (format decoder, quality convertion and manipulation)
* listener functions
* recorder
* sound source
* dynamic circular buffer
* emitter
* advanced player
* equalizer and filters

### Video
* video backend using compute library
* equalizer and filters
* multi-screen support
* desktop recording

### Utilities
* timer
* calendar
* passive timeline with speed adjustment
* fast COW string
* circular queue + atomic
* signals
* reactive functional types
* simd types
* resource management


## Longterm Framework Goals ##
* completely integrate into the std library
* unified dynamic memory management for both host and compute devices
* drop opengl, directx, glew, egl
* drop freetype
* drop openal
* drop sndfile
* transparent filesystem management (using protocols)