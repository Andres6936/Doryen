FIND_PACKAGE(SDL REQUIRED)

IF (NOT SDL_FOUND)
    MESSAGE("Cannot found SDL Version 1.2.15" FATAL_ERROR)
ENDIF ()

MESSAGE("Version found of SDL ${SDL_VERSION_STRING}")

ADD_LIBRARY(Doryen.Core.Cpp
        Algorithms/Generation/Dungeon/BinarySpacePartition.cpp
        Algorithms/Generation/Heightmap.cpp
        Algorithms/Pathfinding/AStar.cpp
        Algorithms/Drawing/Bresenham.cpp
        Algorithms/Noise/Perlin.cpp
        Algorithms/Noise/Noise.cpp
        Algorithms/Util/Cell.cpp
        Algorithms/Util/Node.cpp
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

ADD_LIBRARY(Doryen::Core::Cpp ALIAS Doryen.Core.Cpp)

SET_TARGET_PROPERTIES(Doryen.Core.Cpp PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Doryen.Core.Cpp PRIVATE ${SDL_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(Doryen.Core.Cpp
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Include/>
        $<INSTALL_INTERFACE:../Include/>
        PRIVATE
        ${SDL_INCLUDE_DIR})