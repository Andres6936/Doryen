FIND_PACKAGE(SDL)

IF (NOT SDL_FOUND)
    MESSAGE(FATAL_ERROR "Cannot found SDL Version 1.2.15")
ENDIF ()

MESSAGE(STATUS "Version found of SDL ${SDL_VERSION_STRING}")

ADD_LIBRARY(Doryen.Framework
        Algorithms/Generation/Dungeon/BinarySpacePartition.cpp
        Algorithms/Generation/Heightmap.cpp
        Algorithms/Pathfinding/AStar.cpp
        Algorithms/Drawing/Bresenham.cpp
        Algorithms/Noise/Simplex.cpp
        Algorithms/Noise/Wavelet.cpp
        Algorithms/Noise/Perlin.cpp
        Algorithms/Noise/Noise.cpp
        Algorithms/Noise/FBM.cpp
        Algorithms/Util/Cell.cpp
        Algorithms/Util/Node.cpp
        Graphics/Color/ColorTranslator.cpp
        Graphics/Color/Color.cpp
        Console/Console.cpp
        Console/Char.cpp
        Renderer/Renderer.cpp
        Renderer/Sdl.cpp
        FOV/CircularRaycasting.cpp
        FOV/DiamondRayCasting.cpp
        FOV/ShadowCasting.cpp
        FOV/Permissive.cpp
        FOV/Fov.cpp
        Random/Number.cpp
        Image/PNG/DecompressSettings.cpp
        Image/PNG/HuffmanTree.cpp
        Image/PNG/ColorMode.cpp
        Image/PNG/Adler32.cpp
        Image/PNG/LodePNG.cpp
        Image/ImageData.cpp
        Image/PNG/State.cpp
        Image/PNG/Info.cpp
        Image/Mipmap.cpp
        Image/Image.cpp
        Event/Mouse.cpp
        Event/Key.cpp
        )

ADD_LIBRARY(Doryen::Framework ALIAS Doryen.Framework)

SET_TARGET_PROPERTIES(Doryen.Framework PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Doryen.Framework PRIVATE ${SDL_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(Doryen.Framework
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Include/>
        $<INSTALL_INTERFACE:../Include/>
        PRIVATE
        ${SDL_INCLUDE_DIR})

# Important, the use of CPM as Script for download Doryen brings with it several problems.
# The first problem and most important, is the location of resource Terminal.png, that is
# the font used for default for Doryen, it for it important, set the font (it location)
# to an path where Doryen can be found. For this is needed move Terminal.png to several
# location for to be sure that the resource will be found.
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Terminal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Terminal.png DESTINATION ${CMAKE_BINARY_DIR})