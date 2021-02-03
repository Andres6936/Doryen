# Is important the order of definition, Algorithms need of Random
ADD_SUBDIRECTORY(Random/)
ADD_SUBDIRECTORY(Algorithms/)

# The OBJECT's can be used to compile the sources in the list given to add_library
# to object files, but then neither archiving them into a static library nor
# linking them into a shared object. The use of object libraries is
# particularly useful if one needs to create both static and shared libraries
# in one go.

# Property: OBJECT
ADD_LIBRARY(Doryen.Framework.Object OBJECT
        Algorithms/Generation/Dungeon/BinarySpacePartition.cpp
        Algorithms/Pathfinding/AStar.cpp
        Algorithms/Drawing/Bresenham.cpp
        Algorithms/Util/Cell.cpp
        Algorithms/Util/Node.cpp
        Graphics/Color/ColorTranslator.cpp
        Graphics/Color/Color.cpp
        Console/Console.cpp
        Console/Char.cpp
        Renderer/Renderer.cpp

        # If the renderer to use is SDL version 1, include the files in the process of compilation.
        # This source files are the uniques that must be include headers of SDL, aka <SDL/SDL.h>
        $<$<BOOL:${DORYEN_RENDERER_SDL_1}>:Renderer/Sdl.cpp>
        $<$<BOOL:${DORYEN_RENDERER_SDL_1}>:Renderer/Sdl/ImageSdl.cpp>

        FOV/CircularRaycasting.cpp
        FOV/DiamondRayCasting.cpp
        FOV/ShadowCasting.cpp
        FOV/Permissive.cpp
        FOV/Fov.cpp
        Image/PNG/DecompressSettings.cpp
        Image/PNG/HuffmanTree.cpp
        Image/PNG/ColorMode.cpp
        Image/PNG/Adler32.cpp
        Image/PNG/LodePNG.cpp
        Image/PNG/State.cpp
        Image/PNG/Info.cpp
        Image/MetaImage.cpp
        Image/Mipmap.cpp
        Image/Image.cpp
        Event/Mouse.cpp
        Event/Key.cpp
        )

SET_TARGET_PROPERTIES(Doryen.Framework.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Framework.Object PUBLIC $<BUILD_INTERFACE:${DORYEN_ROOT_DIR}/Include/>)

IF (DORYEN_BUILD_SHARED_LIBS)

    # The POSITION_INDEPENDENT_CODE property determines whether position independent executables or
    # shared libraries will be created. This property is True by default for SHARED and MODULE library
    # targets and False otherwise.

    # Property: SHARED, POSITION_INDEPENDENT_CODE
    ADD_LIBRARY(Doryen.Framework SHARED $<TARGET_OBJECTS:Doryen.Framework.Object>)

ELSE ()

    ADD_LIBRARY(Doryen.Framework STATIC $<TARGET_OBJECTS:Doryen.Framework.Object>)

ENDIF ()

ADD_LIBRARY(Doryen::Framework ALIAS Doryen.Framework)
SET_TARGET_PROPERTIES(Doryen.Framework PROPERTIES CXX_STANDARD 17)
# It is necessary to repeat these instructions again, in case of not doing it, the executables
# will be unable to locate the Headers needed to perform the linking
TARGET_INCLUDE_DIRECTORIES(Doryen.Framework PUBLIC $<BUILD_INTERFACE:${DORYEN_ROOT_DIR}/Include/>)

# Important, the use of CPM as Script for download Doryen brings with it several problems.
# The first problem and most important, is the location of resource Terminal.png, that is
# the font used for default for Doryen, it for it important, set the font (it location)
# to an path where Doryen can be found. For this is needed move Terminal.png to several
# location for to be sure that the resource will be found.
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Terminal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Terminal.png DESTINATION ${CMAKE_BINARY_DIR})

# Configure and link all the need for use SDL as renderer
IF (DORYEN_RENDERER_SDL_1)

    # Locate the SDL library
    FIND_PACKAGE(SDL)

    # Defined by FindSDL: SDL_FOUND, if false, do not try to link to SDL
    IF (NOT SDL_FOUND)
        MESSAGE(FATAL_ERROR "Cannot found SDL Version 1.2.15")
    ENDIF ()

    # Currently the most widespread version of SDL version 1 is version 1.12.15,
    # this version is installed by default using the sudo apt-get
    # Defined by FindSDL: SDL_VERSION_STRING, human-readable string containing the version of SDL
    IF (NOT ${SDL_VERSION_STRING} STREQUAL "1.2.15")
        MESSAGE(FATAL_ERROR "Doryen need SDL 1.2.15 for the renderer SDL \
        version 1, the current version of SDL is ${SDL_VERSION_STRING}
        \nPlease update your version of SDL or use other renderer. See \
        documentation for more details.")
    ENDIF ()

    TARGET_INCLUDE_DIRECTORIES(Doryen.Framework.Object PRIVATE ${SDL_INCLUDE_DIR})
    TARGET_COMPILE_DEFINITIONS(Doryen.Framework.Object PRIVATE DORYEN_USE_SDL_1=1)
    TARGET_LINK_LIBRARIES(Doryen.Framework.Object PRIVATE ${SDL_LIBRARY})

    TARGET_INCLUDE_DIRECTORIES(Doryen.Framework PRIVATE ${SDL_INCLUDE_DIR})
    TARGET_COMPILE_DEFINITIONS(Doryen.Framework PRIVATE DORYEN_USE_SDL_1=1)
    TARGET_LINK_LIBRARIES(Doryen.Framework PRIVATE ${SDL_LIBRARY})

ENDIF ()