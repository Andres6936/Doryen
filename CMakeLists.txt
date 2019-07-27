CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(libtcod)

# All projects need "Include" directory
INCLUDE_DIRECTORIES(Include)

FIND_PACKAGE(OpenGL REQUIRED)

SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(DEPENDENCY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
SET(GUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/gui)

OPTION(LIBTCOD_SAMPLES "Generate libtcod samples" ON)
OPTION(SWIG_CSHARP "Generate CSharp" OFF)
OPTION(SWIG_PYTHON "Generate Python" OFF)
OPTION(SWIG_LUA "Generate Lua" OFF)

IF (MSVC)
    SET(LIBTCOD_NAME libtcod-VS)
    SET(LIBTCOD_GUI_NAME libtcod-gui-VS)
    SET(ZLIB_DIR msvc)
    IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(LIB_DIR msvc64)
    ELSE ()
        SET(LIB_DIR msvc32)
    ENDIF ()
ELSEIF (MINGW)
    SET(LIBTCOD_NAME tcod-mingw)
    SET(LIBTCOD_GUI_NAME tcod-gui-mingw)
    SET(LIB_DIR mingw)
    SET(ZLIB_DIR mingw)
ELSEIF (APPLE)
    SET(LIBTCOD_NAME tcod)
    SET(LIBTCOD_GUI_NAME tcod-gui)
    SET(LIB_DIR osx)
    SET(ZLIB_DIR osx)
ELSEIF (UNIX)
    SET(LIBTCOD_NAME tcod)
    SET(LIBTCOD_CPP_NAME tcodxx)
    SET(LIBTCOD_GUI_NAME tcod-gui)
    SET(LIB_DIR linux)
    SET(ZLIB_DIR linux)
ENDIF ()

#Linux uses system libraries
IF (MSVC OR MINGW)
    # MSVC uses special SDL headers.
    IF (MSVC)
        INCLUDE_DIRECTORIES(${DEPENDENCY_DIR}/SDL-1.2.15/include/msvc)
    ELSE ()
        INCLUDE_DIRECTORIES(${DEPENDENCY_DIR}/SDL-1.2.15/include)
    ENDIF ()

    INCLUDE_DIRECTORIES(${DEPENDENCY_DIR}/zlib-1.2.3/include)

    LINK_DIRECTORIES(${DEPENDENCY_DIR}/SDL-1.2.15/lib/${LIB_DIR}/
            ${DEPENDENCY_DIR}/zlib-1.2.3/lib/${ZLIB_DIR}/)
ELSE ()
    FIND_PACKAGE(X11 REQUIRED)
    FIND_PACKAGE(SDL REQUIRED)
    FIND_PACKAGE(ZLIB REQUIRED)
    FIND_PACKAGE(Threads REQUIRED)
    FIND_LIBRARY(M_LIB m)
    SET(EXTERNAL_LIBS ${X11_LIBRARIES}
            ${SDL_LIBRARY} # this sucks, it's LIBRARY not LIBRARIES
            ${ZLIB_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT} # this comes from find_package(Threads ..)
            ${M_LIB_LIBRARIES}
            ${OPENGL_LIBRARIES}
            m) # This is Math library
    SET(EXTERNAL_INCLUDES ${X11_INCLUDE_DIRS}
            ${SDL_INCLUDE_DIR} # again, singular...
            ${ZLIB_INCLUDE_DIRS}
            ${M_LIB_INCLUDE_DIRS}
            ${X11_INCLUDE_DIRS}
            ${OPENGL_INCLUDE_DIRS})
ENDIF ()

ADD_SUBDIRECTORY(Source) #Needs to come first since samples depend on it

# Use -DLIBTCOD_SAMPLES=OFF if you want do disable the building of the samples (eg. library API changes).
IF (LIBTCOD_SAMPLES)
    ADD_SUBDIRECTORY(samples)
    ADD_SUBDIRECTORY(samples/frost) # Frost effect demo
    ADD_SUBDIRECTORY(samples/navier) # Navier demo
    ADD_SUBDIRECTORY(samples/rad) # Radiosity demo
    ADD_SUBDIRECTORY(samples/ripples) # Water ripples demo
    ADD_SUBDIRECTORY(samples/weather) # Weather + Day/Night demo
    ADD_SUBDIRECTORY(samples/worldgen) # WorldGen demo
ENDIF ()

# For swig to work you either need to set SWIG_PROGRAM_PATH (Windows) or SWIG_SYSTEM (Everywhere else).
# You also need to uncomment the add_subdirectory at the end of the file...

# This needs to be set to the path to a swig executable if you want swig support (and uncommented out).
OPTION(SWIG_ON "Run SWIG" OFF)
SET(SWIG_PROGRAM_PATH "C:\\Users\\donblas\\Programming\\swigwin-1.3.40" CACHE FILEPATH "Path to SWIG")
IF (SWIG_ON)
    SET(SWIG_SYSTEM TRUE)
    ADD_SUBDIRECTORY(swig)
ENDIF ()

