### Doryen Based in Libtcod 1.5.1

Based on the new guidelines taken by Libtcod and based on the state
project, I have decided to take matters into my own hands and 
fork version 1.5.1, which by then had a minimal support for CMake, 
but it still served as a starting point to start the refactoring
and cleaning of the code.

The guidelines taken by Libtcod are:

- _"stopped adding C++ implementations into libtcod."_ [Reference Issue 71](
https://github.com/libtcod/libtcod/issues/71#issuecomment-668205669)

- _"As much as I liked using C++ over C it has made the code less portable 
 and made the C ABI harder to maintain. I now plan on rewriting the code 
 in C and porting to C++ instead of the other way around."_ [Reference Issue
 39](https://github.com/libtcod/libtcod/issues/39#issue-548934343)

This project leaves aside those guidelines taken by Libtcod and
its place took as a starting point the implementation of new functionality
in C++.

### Differences with Libtcod 1.5.1

The code has been completely restructured, modified
the structure of the project to match the recommendations made
by GitLab about [how to structure the project](
https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html), 
several modules have been removed, the API has been changed and full 
support has been given to CMake.

We have chosen to fully support CMake, beyond allowing
the construction of the project on various platforms as to allow the use
this tool as a [Content Package Manager](
https://github.com/TheLartians/CPM.cmake), that will allow you to download
Doryen as a dependency without the need to embed the project within
of each application you want to use it.

### Features

##### Keyboard Input and Mouse Input

The keyboard module supports both turn-by-turn and real time games.

##### True color console

Don’t limit yourself to monochrome or 16 color roguelikes. 
True colors create a whole world of new visual possibilities.

##### Antialiased bitmap font support

Antialiasing makes it possible to have better looking fonts, 
even when you’re using characters as small as 8×8.

##### Sub-cell resolution & SDL callback

Use a few special characters within a font to simulate pixels half 
the size of a console cell or directly apply SDL post-processing on 
the image.

##### High quality pseudo-random number generation

Using Mersenne twister algorithm.

##### BMP and PNG image support

You can blit images onto your console background.

##### Perlin noise toolkit

Support for Perlin, simplex and wavelet noises.

##### Heightmap toolkit

Tools to generate heightmaps.

##### A field of view toolkit

Supported algorithms:

- Basic raycasting.
- Recursive shadowcasting.
- Diamond raycasting.
- Precise permissive fov.
- Restrictive shadowcasting a.k.a. MRPAS.

##### Path finding toolkit

Using A* or Dijkstra algorithms, with either a builtin map data structure 
or your own internal data structure through function callbacks.

##### BSP toolkit

For regions, cities, dungeons building.

### Compiling from source

##### Requirements

- Compiler with Support to C++17 (GCC, Clang, MSV, etc ...)
- CMake Version >= 3.15
- SDL Version === 1.2.15

##### Commands

You can clone with Git via the GitHub repository.

    git clone --recursive --depth=1 https://github.com/Andres6936/Doryen.git
    cd Doryen && mkdir Build
    cd Build && cmake .. -DDORYEN_BUILD_SAMPLES=ON
    make
    

### How to use the library as a downloadable dependency

To use this library, you need the script written by [TheLartians](https://github.com/TheLartians),
named [CPM.cmake](https://github.com/TheLartians/CPM.cmake) (acronym for CMake
Package Manager). Add it to your project, for example, under the
`CMakeModules` and then in your `CMakeFiles.txt` write the following:

- The directory `CMakeModules` is used to store CMake Scripts,
  in case you use another directory for these purposes it will be necessary that
  references properly CPM.cmake to be able to download the dependency.

```cmake
Include(CMakeModules/CPM.cmake)

CPMAddPackage(
  NAME Doryen
  VERSION 2021.0124
  GITHUB_REPOSITORY Andres6936/Doryen
)
 
TARGET_LINK_LIBRARIES(<TARGET> PRIVATE Doryen::Framework)
```

###### Please, in the VERSION option you should preferably specify the latest version available consult [here](https://github.com/Andres6936/Doryen/releases/latest)

###### All screenshots come from the Doryen sample source code.

<details>
<summary>Click for See Screenshots</summary>
    <p align="center">
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/TrueColors.png" alt="A"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/OffscreenConsole.png" alt="B"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/LineDrawing.png" alt="C"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/Noise.png" alt="D"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/FieldOfView.png" alt="E"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/PathFinding.png" alt="F"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/BSPToolkit.png" alt="G"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/ImageToolkit.png" alt="H"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/MouseSupport.png" alt="I"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/NameGenerator.png" alt="J"/>
        <img src="https://raw.githubusercontent.com/Andres6936/Doryen.Documentation/master/Image/Screen/SDLCallback.png" alt="K"/>
    </p>
</details>
