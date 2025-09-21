# C++ Raytracing Engine – Visualization of Rendering Techniques


This project is a custom-built graphics engine in C++ developed to explore and visualize core ray-based rendering techniques. 
Initially based on a simple GUI that displayed only a solid background color, the engine has been extended to support raycasting, raytracing, and path tracing. 
It also features optical effects such as transparency, metallic reflections (mirror surfaces), and texture mapping.

In addition to geometry and lighting, the engine incorporates multiple shading models to simulate different visual styles, including normal shading, diffuse shading, Blinn-Phong, and toon shading. 
The main goal is to provide a hands-on understanding of how various rendering and shading techniques interact to produce realistic and stylized images in computer graphics.

## Project Overview

The file `Project_report` documents the full development process of the raytracing engine — from the initial stage, where only a solid background was rendered, 
to the current state with complex scenes, multiple objects, and advanced rendering techniques.


## How it works

The rendering process begins in the `World.cpp` class, specifically in the `renderWorld()` method. Configuration settings can be adjusted either directly through the `Config.hpp` file or via the built-in GUI.

To explore different scenes, you can modify the `init()` method within the `Scene.cpp` class or define your own custom scenes. 
This flexible architecture allows for experimentation with various rendering setups and visual effects.

