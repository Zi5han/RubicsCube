# RubicsCube

*RubicsCube* is a 3D puzzle simulation built with *C++* and *OpenGL*, developed by **Zishan Ahmed** under the supervision of **Prof. Dr. Christoph Lürig** at Trier University of Applied Sciences.

The project implements an interactive Rubik's Cube with smooth 3D rotation, face manipulation, and custom input handling — showcasing rendering, animation, and mathematical logic within an OpenGL framework.  

*(the spelling mistake in the projects name is intentional lol)*

<img width="1680" height="1050" alt="Screenshot of RubicsCube" src="https://github.com/user-attachments/assets/66efc108-1806-4458-8699-4d2bd4060f7b" />

---

## Overview

The simulation allows users to rotate and interact with a virtual Rubik’s Cube using mouse and keyboard input.  
It focuses on clean architecture, modular systems, and mathematical precision rather than visual complexity.

## Key Features

- Fully interactive 3×3×3 Rubik’s Cube  
- Real-time 3D rotation and snapping animations  
- Custom mouse and keyboard input system  
- Model loading from OBJ/MTL files  
- Optimized OpenGL rendering

## Controls

### **Keyboard (QWERTZ/QWERTY layout) \[always active\]**
- `Right mouse button (holding)` - Rotate cube  
- `Left mouse button (holding)` - Rotate a layer  
- `Mouse wheel` - Zoom in/out
- `Space` - Reset cube

## Built With
- **Visual Studio 2023 (C++)**
- **OpenGL / GLFW** - with custom input & render systems  
- **GLM** – Math library  

## How to build
1. Open the solution `RubiksCubeProject/RubiksCubeProject.sln` in *Microsoft Visual Studio*
2. Start building process of the configuration *Debug x64*
3. Start the compiled exe file

Note: Only supported on Windows (tested on Windows 10 & 11)

---

## Author
**Zishan Ahmed**

## Acknowledgments
Special thanks to my supervisor **Prof. Dr. Christoph Lürig** at *Trier University of Applied Sciences* for the consultation and assistance

## License
All code and assets are published with the agreement of me, **Zishan Ahmed**.  
Licensed under MIT — see [LICENSE](./LICENSE).
