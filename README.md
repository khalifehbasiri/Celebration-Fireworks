# Celebration Fireworks

A C++/OpenGL graphics project that simulates fireworks above a textured landscape with procedurally generated trees and a night skybox.

## Features

- Automatic firework spawning, with 800 particles per burst.
- Particle motion with gravity, drag, color, and lifetime-based fading.
- Recursive tree geometry with randomized branches and placement.
- Textured ground and a six-image skybox.
- Keyboard controls for camera movement, rotation, zoom, and wireframe rendering.

## Build and run

### Requirements

- Windows and Visual Studio with the C++ desktop workload, MSVC v143 toolset, and a Windows SDK.
- A graphics driver that supports OpenGL 3.3 / GLSL 3.30.
- GLEW, FreeGLUT, and GLM. Use Win32 (x86) libraries for the supplied project configurations.
- The course framework headers, including `GL/nuss_math.h`, `GL/nuss_matrix.h`, and their dependencies. These are **not included** in this repository.

`stb_image.h` is included for image loading.

### Visual Studio setup

1. Open `skybox.vcxproj` directly in Visual Studio; no solution file is included.
2. Select **Debug | Win32**. This configuration contains the dependency paths and linker entries; Release still needs equivalent setup.
3. Define the `GL_Shared_Location` environment variable or MSBuild property to point to your graphics dependency directory. The Debug configuration expects:

   ```text
   <GL_Shared_Location>/
     include/
       GL/       # GLEW, GLUT, and course framework headers
       glm/      # GLM headers
     lib/Win32/  # x86 import libraries
     bin/Win32/  # Matching runtime DLLs
   ```

   If you set an environment variable while Visual Studio is open, restart Visual Studio so it picks up the value. Alternatively, adjust the project's include and library paths to match your installation.
4. Check **Linker > Input > Additional Dependencies**. The Debug project lists `freeglut.lib` and `glew32.lib`; ensure `opengl32.lib` is also linked.
5. Set **Debugging > Working Directory** to `$(ProjectDir)`. Shaders and textures are loaded by relative filename from the repository root. The supplied `.vcxproj.user` file adds `$(GL_Shared_Location)\bin\$(PlatformName)` to the Debug runtime `PATH`.
6. Build and run the project. Fireworks spawn automatically; there is no manual launch control.

The current code has rendering and resource-management issues, so a successful build does not guarantee a correct display. See the limitations below.

## Controls

Letter controls are case-sensitive unless both forms are listed.

| Key | Action |
| --- | --- |
| `w` / `s` | Move forward / backward |
| `i` / `k` | Move up / down |
| `a` / `d` | Turn left / right (yaw) |
| `g` / `G` (Shift + G) | Move right / left |
| Up / Down arrows | Pitch up / down |
| Left / Right arrows | Roll the camera |
| `z` / `Z` (Shift + Z) | Zoom in / out |
| `1`-`9` | Set the movement and rotation speed multiplier |
| `p` | Toggle wireframe rendering; currently also changes speed due to a switch fallthrough |
| Escape, `q`, or `Q` | Quit |

## Project layout

| File(s) | Purpose |
| --- | --- |
| `skybox_main.cpp` | Application entry point and GLUT main loop |
| `Solution.cpp`, `Solution.h` | Scene setup, input, rendering, and update callbacks |
| `Fireworks.cpp`, `Fireworks.h` | Particle initialization, simulation, and rendering |
| `Tree.cpp`, `Tree.h` | Recursive tree mesh generation |
| `camera.cpp`, `camera.h` | Camera movement and projection |
| `Shader.*`, `Texture.*`, `SkyBox.*`, `graphicsObject.*` | Rendering helpers |
| `*.vs`, `*.fs`, `*.geom` | Shader sources; the firework geometry shader is not currently loaded |
| `*.png`, `*.jpg` | Texture assets |
| `skybox.vcxproj` | Visual Studio project with Debug and Release Win32 configurations |

## Known limitations

- Firework rendering binds a `fireVAO` that is never initialized, and particles bind an unloaded per-particle texture instead of the loaded firework texture.
- Fireworks start at height `1000`, above the explosion threshold of `320`, so they explode on the first update.
- Expired firework objects stay in the collection, and OpenGL textures and buffers are not released.
- Resizing updates the viewport but does not update the camera's aspect ratio.
- Environment reflections and lighting driven by firework bursts are not implemented as a complete scene effect.

## Credits and usage

The project uses OpenGL, GLEW, FreeGLUT, GLM, and the bundled stb image loader. Several framework files credit Doron Nussbaum and include educational-use restrictions. Refer to the notices in those source files and the dependencies' own licenses for their terms.
