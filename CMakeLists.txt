CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(libtcod)

# All projects need "Include" directory
INCLUDE_DIRECTORIES(Include)

FIND_PACKAGE(OpenGL REQUIRED)

SET(DEPENDENCY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
SET(GUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/gui)

OPTION(LIBTCOD_SAMPLES "Generate libtcod samples" ON)

SET(LIBTCOD_NAME Doryen)
SET(LIBTCOD_GUI_NAME DoryenUI)

IF (MSVC)

    SET(ZLIB_DIR msvc)

    IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(LIB_DIR msvc64)
    ELSE ()
        SET(LIB_DIR msvc32)
    ENDIF ()

ELSEIF (MINGW)

    SET(LIB_DIR mingw)
    SET(ZLIB_DIR mingw)

ELSEIF (APPLE)

    SET(LIB_DIR osx)
    SET(ZLIB_DIR osx)

ELSEIF (UNIX)

    SET(LIBTCOD_CPP_NAME tcodxx)

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
