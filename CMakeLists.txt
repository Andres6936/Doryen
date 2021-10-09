CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)
PROJECT(Doryen VERSION 2021.0226 LANGUAGES CXX)

# The shared libraries are libraries that can be linked dynamically and loaded at runtime.
OPTION(DORYEN_BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" OFF)
OPTION(DORYEN_BUILD_SAMPLES "Generate the Doryen Samples" OFF)
OPTION(DORYEN_BUILD_TESTING "Generate and Execute the Doryen Test" OFF)

# Render to use by default
OPTION(DORYEN_RENDERER_SDL_1 "Use the SDL backend" ON)
# Render based in the SFML 2.5 framework
OPTION(DORYEN_RENDERER_SFML_25 "Use the SFML 2.5 backend" OFF)
# The renderer is used for testing purpose
OPTION(DORYEN_RENDERER_DUMMY "Use the dummy backend" OFF)

# For default, the SDL 1 renderer is ON, but if is defined any other renderer the renderer SDL 1
# will be included in the code and produce compiler error to not found the headers in machines
# where the SDL renderer not will used.
IF (DORYEN_RENDERER_DUMMY)

    # Set the SDL 1 renderer to OFF for avoid use this renderer
    SET(DORYEN_RENDERER_SDL_1 OFF)

ELSEIF(DORYEN_RENDERER_SFML_25)

    # Set the SDL 1 renderer to OFF for avoid use this renderer
    SET(DORYEN_RENDERER_SDL_1 OFF)

ENDIF ()

# Set the root directory where can be found the directory Include/
# needed for set inclusion guards and more
SET(DORYEN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Needs to come first since Samples depend on it
ADD_SUBDIRECTORY(Source)

# Use -DLIBTCOD_SAMPLES=OFF if you want do disable the building of the Samples (eg. library API changes).
IF (DORYEN_BUILD_SAMPLES)
    ADD_SUBDIRECTORY(Samples/CPP)
    ADD_SUBDIRECTORY(Samples/Frost)
    ADD_SUBDIRECTORY(Samples/Images)
    ADD_SUBDIRECTORY(Samples/Navier)
    ADD_SUBDIRECTORY(Samples/Rad)
    ADD_SUBDIRECTORY(Samples/Ripples)
    ADD_SUBDIRECTORY(Samples/Weather)
    ADD_SUBDIRECTORY(Samples/Worldgen)
ENDIF ()

IF (DORYEN_BUILD_TESTING)
    # How do I use CTest? Simple: you first tell CMake to enable CTest by adding
    # the following line somewhere near the top of you main CMakeLists.txt
    # (usually right after the required cmake_minimum_required() and project())
    # Reference: https://bertvandenbroucke.netlify.app/2019/12/12/unit-testing-with-ctest/
    ENABLE_TESTING()

    ADD_SUBDIRECTORY(Test)
ENDIF ()
