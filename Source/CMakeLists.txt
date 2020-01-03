IF (MSVC)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME zdll)

ELSEIF (MINGW)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME z)

ELSEIF (APPLE)

    FIND_PACKAGE(SDL REQUIRED)
    SET(SDL_NAME ${SDL_LIBRARY})
    SET(ZLIB_NAME z)

ELSEIF (UNIX)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME z)

ENDIF ()

SET(LIBTCOD_C_SRC color_c.c console_c.c fov_c.c
        fov_diamond_raycasting.c fov_permissive2.c
        fov_restrictive.c image_c.c lex_c.c list_c.c mersenne_c.c namegen_c.c
        noise_c.c parser_c.c sys_c.c sys_opengl_c.c sys_sdl_img_bmp.c
        tree_c.c zip_c.c SystemSDL.c)

SET(LIBTCOD_CPP_SRC Algorithms/Drawing/Bresenham.cpp Algorithms/Generation/Dungeon/BinarySpacePartition.cpp Color.cpp
        Console.cpp FOV/Fov.cpp Algorithms/Generation/Heightmap.cpp Image/Image.cpp Image/ImageData.cpp Image/Mipmap.cpp
        mersenne.cpp mouse.cpp namegen.cpp noise.cpp path.cpp Platform/Platform.cpp Renderer/Sdl.cpp zip.cpp
        FOV/CircularRaycasting.cpp FOV/DiamondRayCasting.cpp FOV/Permissive.cpp FOV/Restrictive.cpp FOV/ShadowCasting.cpp
        Cell.cpp Algorithms/Util/Node.cpp Algorithms/Pathfinding/AStar.cpp
        Image/PNG/LodePNG.cpp Image/PNG/Info.cpp SystemSDLImagePNG.cpp)

SET(LIBTCOD_GUI_SRC gui/button.cpp gui/container.cpp gui/flatlist.cpp gui/hbox.cpp gui/Image.cpp gui/label.cpp gui/radiobutton.cpp
        gui/slider.cpp gui/statusbar.cpp gui/textbox.cpp gui/togglebutton.cpp gui/toolbar.cpp gui/vbox.cpp gui/widget.cpp)

# Linux wants seperate C and C++ libraries.
IF (UNIX AND NOT APPLE)
    INCLUDE_DIRECTORIES(${EXTERNAL_INCLUDES})
    ADD_LIBRARY(${LIBTCOD_NAME} SHARED ${LIBTCOD_C_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_NAME} ${EXTERNAL_LIBS})

    ADD_LIBRARY(${LIBTCOD_CPP_NAME} SHARED ${LIBTCOD_CPP_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_CPP_NAME} ${LIBTCOD_NAME} ${EXTERNAL_LIBS})
ELSE ()
    ADD_LIBRARY(${LIBTCOD_NAME} SHARED ${LIBTCOD_C_SRC} ${LIBTCOD_CPP_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_NAME} ${SDL_NAME} ${ZLIB_NAME} ${OPENGL_LIBRARY})
ENDIF ()

INCLUDE_DIRECTORIES(${GUI_INCLUDE_DIR})
ADD_LIBRARY(${LIBTCOD_GUI_NAME} SHARED ${LIBTCOD_GUI_SRC})
TARGET_LINK_LIBRARIES(${LIBTCOD_GUI_NAME} ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})  #${LIBTCOD_CPP_NAME} is empty on non-linux platforms

IF (MSVC OR MINGW)
    # Since we're exporting functions and clases, we use the "standard" macro combinations to do so under msvc.
    # These turn them on for the right builds.
    SET_PROPERTY(TARGET ${LIBTCOD_NAME} PROPERTY COMPILE_DEFINITIONS LIBTCOD_EXPORTS)
    SET_PROPERTY(TARGET ${LIBTCOD_GUI_NAME} PROPERTY COMPILE_DEFINITIONS LIBTCOD_GUI_EXPORTS)
ENDIF ()
