# C++  Z-Buffer Engine – Visualization of Rasterization Techniques

This project is a custom-built graphics engine in C++ developed to explore and demonstrate core rasterization-based rendering techniques using a traditional z-buffer pipeline.
Initially built with a simple GUI that displayed only a solid background color, the engine has been expanded to support scene transformations, per-object manipulation, multiple lighting types, and various shading models executed on the GPU.

The engine includes support for lighting techniques such as ambient and point lights, GPU-based material handling, and several shading models including normal shading, Phong, Gouraud, toon shading, and silhouette emphasis.
Additionally, it features support for texturing, texture-per-object assignment, indirect mapping, and optional non-photorealistic effects.

The main goal is to provide a hands-on understanding of how the rasterization pipeline operates and how different lighting and shading models can be combined to produce both realistic and stylized visual outputs in real-time graphics.

## Project Overview

The file `Project_report` file documents the full development process of the z-buffer-based engine. From the initial stage with just a static background to the current version featuring complex scenes, multiple objects, and GPU-accelerated rendering techniques.

## How it works

The rendering process is handled in the `GLWidget` class, primarily in the `paintGL()` method, which is responsible for clearing the buffers and invoking the scene drawing logic via the `world->draw()` call. 
All OpenGL features such as depth testing and face culling are configured in `setupOpenGLFeatures()`, and custom shaders are initialized in `initShadersGPU()`.

Scene initialization and GPU setup take place in `initWorld()`, where the camera, light manager, and scene graph are instantiated. 
Shader programs can be dynamically activated using `activateShader()`, which updates lighting, material, and texture configurations depending on the selected shading model.

User interaction is handled via mouse events, enabling either object rotation or translation based on the active control mode. 
These transformations are applied to the latest added object through the transformation matrix computed in `mouseMoveEvent()`.

Additional functionality includes dynamic object loading via `loadObject()` (from file) or `addCube()` (procedural), 
each followed by a GPU update and application of transformation states. The GUI can adjust rendering modes, light properties, and global ambient color, offering a flexible framework to experiment with real-time shading and rasterization techniques.

