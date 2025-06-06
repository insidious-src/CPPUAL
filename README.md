![Logo](https://raw.githubusercontent.com/insidious-src/CPPUAL/master/cppual.png "C++ Unified Abstraction Library")

# C++ Unified Abstraction Library #

    Latest Supported Language Standard: C++20
    Future Language Standard: C++23

## Framework Goals ##

#### Why reinvent the wheel and replace parts of the standard library and Qt? ####
- the whole idea is to implement everything as a host and compute devices for parallelism,
  deprecating OpenGL, DirectX, VA, VDPAU, OVD, XVideo, TrueAudio and OpenAL completely.
- behaviour-based device processing and explicit events

**So the real question is:** Why would you want to have all these libraries
**when OpenCL is pushing hard and you can program it pretty much like a regular CPU?
**It's just a matter of time until OpenCL evolves and turns into an all-in-one standard.

**March 6th 2015:** Well,... it evolved into Vulkan (GLNext),
**however OpenCL still remains as a new version was released (3.0).
**Both are using a common IL - SPIR-V.


## Concept Goals ##

- A library imitating unified low-level brain communication system


## Technical Specifications ##

### Host ###
* modules **(Done)**
* utilities **(Done)**
* cross-platform process management and interprocess communication **(~60%)**
* query hardware and os information **(5%)**

### Compute ###
* use concurrency as host implementation **(Done)**
* specialized device & shared memory allocators **(~60%)**
* unified memory management & allocator patterns **(~50%)**
* host/device out-of-order and continuation tasks **(~50%)**
* use Vulkan & OpenCL as SINGLE SOURCE backends (SPIR-V is required to implement eventual C++ interpreter) **(~40%)**
* behaviour-based device scheduling and manipulation **(WIP)**

### Active Model View Controller ###
* node based + smart pointer web
* the model not only contains data but also logic
* a logic node can be bound to single or multiple data nodes
* cyclic binding (actually the idea is to ABUSE it)
* can generate multiple model repositories
* interprocess interface
* the interface is protocol agnostic (can exchange data across networks)
* controllers are represented as signal structures + configuration data to interact with the model logic
* the whole pattern can be used to form a universal communication method for almost any type of program
* can generate user interfaces on the fly using layout data without requiring any ui library

### Network ###
* implementation of the C++17 network library **(~80%)**
* simplified packet exchange **(70)**
* layered protocol interface **(~20%)**
* http, https, ftp, bittorrent protocols

### Universal Database Interface ###
* based on AMVC
* standard & relational database model
* DOM model

### Input ###
* specialized raw input queues **(WIP)**
* integrated security
* all types of keyboards
* all types of mice
* generic controller interface & support
* wheel controller
* Xbox 360/One/Series X/Series S controller
* Playstation 1/2/3/4/5 controller
* Steam controller
* extended button layouts
* touch surfaces

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

### Utilities ###
* timer **(Done)**
* calendar
* passive timeline with speed adjustment **(95%)**
* SSO fast string **(Done)**
* rope using SSO fast string
* circular queue + atomic **(WIP)**
* atomic uniform circular queue **(WIP)**
* signals **(Done)**
* reactive functional types **(Done)**
* resource management **(50%)**


## Longterm Framework Goals ##
* completely integrate into the std library **(~40%)**
* unified dynamic memory management for both host and compute devices **(~20%)**
* drop opengl, directx, glew, egl, glx, wgl, agl **(WIP)**
* drop freetype **(WIP)**
* drop openal **(WIP)**
* drop sndfile
* transparent filesystem management (using protocols)