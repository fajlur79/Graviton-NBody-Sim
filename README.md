
<h1 align="center">Graviton 🌀 </h1>

<div align="center">

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![GNU Make](https://img.shields.io/badge/GNU%20Make-0A0A0A.svg?style=for-the-badge&logo=GNU-Make&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

</div>

## Overview

**Graviton** is a high-performance, general-purpose physics engine implemented in C, designed for computationally intensive N-Body simulations. By offloading heavy gravitational calculations to the GPU via custom compute shaders, it provides scalable, real-time physics modeling.


<div align="center">
  <video src="https://github.com/user-attachments/assets/5cfe4e70-9d71-4887-b6eb-93c5b405f6f0" width="800" autoplay loop muted></video>
</div>



## Features
 
- GPU-accelerated force computation via an OpenGL compute shader (`gravity.glsl`),
  dispatched from `physics.c`
- 15,000-particle galaxy by default, initialized in `init.c`
- Free-fly and third-person camera modes
- Soft circular particle sprites generated procedurally at startup
- Fixed-timestep physics loop decoupled from render framerate, with interpolated
  rendering between physics steps
- Optional launcher (`launch`) for picking integrated GPU, discrete AMD, or discrete
  NVIDIA rendering via environment variables before starting the sim
## Requirements
 
- Linux (the Makefile and launcher use Linux-specific APIs — `X11`, `DRI_PRIME`, POSIX
  `unsetenv`/`setenv`/`execvp`)
- GCC with OpenMP support
- [raylib](https://www.raylib.com/) installed (development headers + library)
- OpenGL, GLX, and X11 development libraries


*For Arch Linux*
```sh
sudo pacman -S base-devel raylib
```
*For Ubuntu/Debian*
```sh
sudo apt update
sudo apt install build-essential libraylib-dev
```
## Build
 
```sh
make
```
 
This builds two binaries:
- `graviton` — the simulation itself (`main.c`, `physics.c`, `render.c`, `init.c`)
- `launch` — a small GPU-selection launcher (`launcher.c`)
`make clean` removes build artifacts.
 
## Run
 
Either run the simulation directly:
 
```sh
./graviton
```
 
Or use the launcher to choose a GPU backend first (sets `DRI_PRIME` or
`__NV_PRIME_RENDER_OFFLOAD` accordingly before exec'ing into `graviton`):
 
```sh
./launch
```
 
### Controls
 
| Key | Action |
|-----|--------|
| `C` | Toggle between third-person and free-fly camera |
| `R` | Reset the galaxy to its initial configuration |
| `Q` | Quit |
 
## How it works
 
Each particle (`types.h`) stores position, velocity, acceleration, previous position,
mass, radius, color, and a fixed/pinned flag, laid out for direct upload to a GPU
shader storage buffer. Every physics step, `compute_forces()` in `physics.c` dispatches
`gravity.glsl` as a compute shader to update all particle accelerations and velocities
on the GPU, then reads the results back for the CPU-side integration and rendering in
`render.c`. The main loop in `main.c` uses a fixed-timestep accumulator so physics
stays consistent regardless of render framerate, with `alpha` used to interpolate
particle positions between physics steps for smooth rendering.
 
Simulation parameters (gravitational constant, timestep, particle count, softening
factor, bounds, and a `theta` field reserved for potential tree-approximation tuning)
are configured in the `SimConfig` struct, currently set directly in `main.c`.
 
## Project structure
 
```
main.c        - window/camera setup, particle texture generation, main loop
physics.c     - GPU compute shader setup and dispatch for force integration
render.c      - particle rendering
init.c        - initial galaxy particle generation
launcher.c    - GPU-backend picker / launcher UI
gravity.glsl  - GLSL compute shader for gravitational force calculation
types.h       - Particle and SimConfig struct definitions
Makefile      - build rules for `graviton` and `launch`
```
 
## License
 
This project is licensed under the terms specified in the [LICENSE](LICENSE) file.
 
---