# DirectX_Game
This tech demo aims to show the use of some of the most common DirectX techniques such as rendering models following its correct pipeline, texturing, lighting, reflections, collisions, UI text, and much more. All this is demonstrated through a playable demo with objectives.

## Table of contents
* [Setup](#setup)
* [General info](#general-info)
    * [Intro](#intro)
    * [Controls](#controls)
    * [Game Architecture](#game-architecture)
* [Technologies](#technologies)

## Setup
If any issue running the project occour, please download and install the DirectX SDK from the [Microsoft website](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

## General info
#### Intro
The objective is to destroy the security cameras that are on the other side of the map, trying to avoid or kill the guards patrolling the area. The security cameras deal damage if the player is too close, as well as the guards.

#### Controls:
* Movement: WASD
* Camera rotation: Mouse 
* Shooting: Lef Click
* Jump: SPACEBAR 

#### Game Architecture
The only object instantiated in the main is the App. All the game logic is handled there. 
The constructor initialises all the necessary data, such as the window, all the bindable, the input, the map, and all the entities including the player. The map is loaded from a .txt file that specifies the position of the object and what to instantiate by an alphanumeric code. 
The game loop checks for a “QUIT” message and returns a std::optional<int>, otherwise it returns an empty one and runs the game frame, which updates the logic first and then the rendering.
The framerate is kept in control by the FrameTimer class and provides the delta time necessary to get frame-independent actions. 
The Win condition is given by the number of cameras destroyed. If the player destroys them all, a UI text is rendered telling the player that he has won.

## Technologies 
* DirectX 11
* DirectInput 8
* Win32
