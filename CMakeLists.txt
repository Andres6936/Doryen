CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)
PROJECT(Doryen VERSION 2020.0824 LANGUAGES CXX)

OPTION(DORYEN_SAMPLES "Generate the Doryen Samples" OFF)
OPTION(DORYEN_TESTS "Generate and Execute the Doryen Test" OFF)

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