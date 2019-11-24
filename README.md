# CITM_3_GameEngine
A 3D game engine created in the third year of our Videogame Design and Development degree.

# Instructions:

- Drag an fbx to the scene to create a hirearchy of gameobjects that represents it.

## Mouse picking
- Click on any object with a mehs

## Frustrum culling
- Go to "Configuration" panel
- Open "Camera3D"
- Check "See frustrum culling"
- Rotate and move the main camera
(Note that you must have gameobjects with bounding boxes in the scene to see the effect of the frustrum culling).

## Time management
- Press Play to play the game.
- Press Pause to pause the game.
- Press Pause again to unpause the game.

## Resource management
- You can see the resource count of each resource in the "Resources" panel.

## Scene serialization
- Drag a scene from the Assets panel to the Scene panel to load it.
- Go to File > Save Scene to save scenes.

# Innovation

## Assignment 2:
- Shortcuts panel
- Show scene in an imgui panel
- Each panel is a viewport that can be dettached from the main window
- Event system to notify multiple modules. It has two systems, one that works with a queue and the other that lets you send events immediately to all modules.
- Game panel separate from the scene window.
- File system using PhysFs.
- Octree.

# Contribution

## Assignment 2:
Yessica Servin Dominguez
- Frustrum culling
- Octree
- Scene serialization
- Mouse picking

Jaume Montagut Guix
- Resource Manager
- Own file format

# Credits:

- Yessica Servin Dominguez
https://www.linkedin.com/in/yessica-servin-dominguez/
https://github.com/YessicaSD

- Jaume Montagut Guix
www.linkedin.com/in/jaumemontagut
https://github.com/JaumeMontagut

## Find respository at:
https://github.com/YessicaSD/CITM_3_GameEngine

## Libraries used:
- SDL
https://www.libsdl.org/
- Dear ImGui: To create the GUI
https://github.com/ocornut/imgui
- Glew
http://glew.sourceforge.net/
- Parson: To pase .json files
https://github.com/kgabis/parson
- OpenGL 3

# License:

Copyright 2019 Yessica Servin y Jaume Montagut

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.