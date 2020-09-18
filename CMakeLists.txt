CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)
PROJECT(Doryen VERSION 2020.0901 LANGUAGES CXX)

# The shared libraries are libraries that can be linked dynamically and loaded at runtime.
OPTION(DORYEN_SHARED_LIBRARIES "Generate Shared Libraries" OFF)
OPTION(DORYEN_SAMPLES "Generate the Doryen Samples" OFF)
OPTION(DORYEN_TESTS "Generate and Execute the Doryen Test" OFF)

# Set the root directory where can be found the directory Include/
# needed for set inclusion guards and more
SET(DORYEN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Needs to come first since Samples depend on it
ADD_SUBDIRECTORY(Source)

# Use -DLIBTCOD_SAMPLES=OFF if you want do disable the building of the Samples (eg. library API changes).
IF (DORYEN_SAMPLES)
    ADD_SUBDIRECTORY(Samples/CPP)
    ADD_SUBDIRECTORY(Samples/Frost)
    ADD_SUBDIRECTORY(Samples/Navier)
    ADD_SUBDIRECTORY(Samples/Rad)
    ADD_SUBDIRECTORY(Samples/Ripples)
    ADD_SUBDIRECTORY(Samples/Weather)
    ADD_SUBDIRECTORY(Samples/Worldgen)
ENDIF ()

IF (DORYEN_TESTS)
    ADD_SUBDIRECTORY(Test)
ENDIF ()