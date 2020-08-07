ADD_LIBRARY(Doryen.Core.Cpp
        Algorithms/Generation/Dungeon/BinarySpacePartition.cpp
        Algorithms/Generation/Heightmap.cpp
        Algorithms/Pathfinding/AStar.cpp
        Algorithms/Drawing/Bresenham.cpp
        Algorithms/Noise/Perlin.cpp
        Algorithms/Noise/Noise.cpp
        Algorithms/Util/Cell.cpp
        Algorithms/Util/Node.cpp
        Console/Console.cpp
        Console/Char.cpp
        Graphics/Color/Color.cpp
        Image/ImageData.cpp
        Image/Image.cpp
        Image/Mipmap.cpp
        Renderer/Sdl.cpp
        FOV/CircularRaycasting.cpp
        FOV/DiamondRayCasting.cpp
        FOV/Permissive.cpp
        FOV/Restrictive.cpp
        FOV/ShadowCasting.cpp
        FOV/Fov.cpp
        Random/Number.cpp
        Image/PNG/LodePNG.cpp
        Image/PNG/Info.cpp
        Image/PNG/State.cpp
        Image/PNG/ColorMode.cpp
        Image/PNG/DecompressSettings.cpp
        Image/PNG/HuffmanTree.cpp
        Image/PNG/Adler32.cpp
        Renderer/Renderer.cpp
        Event/Key.cpp
        Event/Mouse.cpp
        )

ADD_LIBRARY(Doryen::Core::Cpp ALIAS Doryen.Core.Cpp)

SET_TARGET_PROPERTIES(Doryen.Core.Cpp PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Doryen.Core.Cpp PRIVATE ${EXTERNAL_LIBS})
TARGET_INCLUDE_DIRECTORIES(Doryen.Core.Cpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Include/>
        $<INSTALL_INTERFACE:../Include/>
        PRIVATE
        ${EXTERNAL_INCLUDES})