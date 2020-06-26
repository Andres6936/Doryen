# Library for C

ADD_LIBRARY(Doryen.Core.C
        color_c.c console_c.c fov_c.c fov_diamond_raycasting.c fov_permissive2.c
        fov_restrictive.c image_c.c lex_c.c list_c.c mersenne_c.c namegen_c.c
        noise_c.c parser_c.c sys_c.c sys_opengl_c.c sys_sdl_img_bmp.c
        tree_c.c zip_c.c SystemSDL.c)

SET_TARGET_PROPERTIES(Doryen.Core.C PROPERTIES C_STANDARD 11)
TARGET_LINK_LIBRARIES(Doryen.Core.C PRIVATE ${EXTERNAL_LIBS})
TARGET_INCLUDE_DIRECTORIES(Doryen.Core.C PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>
        $<INSTALL_INTERFACE:Include/>
        PRIVATE
        ${EXTERNAL_INCLUDES})

# Library for C++

ADD_LIBRARY(Doryen.Core.Cpp
        Algorithms/Drawing/Bresenham.cpp
        Algorithms/Generation/Dungeon/BinarySpacePartition.cpp Color.cpp
        Console.cpp FOV/Fov.cpp Algorithms/Generation/Heightmap.cpp Image/Image.cpp Image/ImageData.cpp
        Image/Mipmap.cpp
        mersenne.cpp mouse.cpp namegen.cpp noise.cpp Platform/Platform.cpp Renderer/Sdl.cpp zip.cpp
        FOV/CircularRaycasting.cpp FOV/DiamondRayCasting.cpp FOV/Permissive.cpp FOV/Restrictive.cpp
        FOV/ShadowCasting.cpp
        Cell.cpp Algorithms/Util/Node.cpp Algorithms/Pathfinding/AStar.cpp
        Image/PNG/LodePNG.cpp Image/PNG/Info.cpp Image/PNG/State.cpp
        Image/PNG/ColorMode.cpp Image/PNG/DecompressSettings.cpp Image/PNG/HuffmanTree.cpp
        Image/PNG/Adler32.cpp SystemSDLImagePNG.cpp Renderer/Renderer.cpp Char.cpp
        Event/Key.cpp Event/Mouse.cpp)

SET_TARGET_PROPERTIES(Doryen.Core.Cpp PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Doryen.Core.Cpp PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Doryen.Core.Cpp PRIVATE ${EXTERNAL_LIBS})
TARGET_INCLUDE_DIRECTORIES(Doryen.Core.Cpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>
        $<INSTALL_INTERFACE:Include/>
        PRIVATE
        ${EXTERNAL_INCLUDES})

# Library for Graphics User Interface

ADD_LIBRARY(Doryen.GUI
        gui/button.cpp gui/container.cpp gui/flatlist.cpp gui/hbox.cpp
        gui/Image.cpp gui/label.cpp gui/radiobutton.cpp
        gui/slider.cpp gui/statusbar.cpp gui/textbox.cpp gui/togglebutton.cpp
        gui/toolbar.cpp gui/vbox.cpp gui/widget.cpp)

SET_TARGET_PROPERTIES(Doryen.GUI PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Doryen.GUI PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Doryen.GUI PRIVATE Doryen.Core.Cpp)
TARGET_INCLUDE_DIRECTORIES(Doryen.GUI PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>
        $<INSTALL_INTERFACE:Include/>
        PRIVATE
        ${EXTERNAL_INCLUDES})