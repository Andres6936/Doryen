CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(Doryen VERSION 2020.0804 LANGUAGES CXX C)

# All projects need "Include" directory

OPTION(DORYEN_SAMPLES "Generate Doryen Samples" ON)
OPTION(DORYEN_TESTS "Execute Doryen Test" ON)

FIND_LIBRARY(M_LIB m)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

SET(EXTERNAL_LIBS
        ${SDL_LIBRARY} # this sucks, it's LIBRARY not LIBRARIES
        ${CMAKE_THREAD_LIBS_INIT} # this comes from find_package(Threads ..)
        ${M_LIB_LIBRARIES}
        m) # This is Math library
SET(EXTERNAL_INCLUDES
        ${SDL_INCLUDE_DIR} # again, singular...
        ${M_LIB_INCLUDE_DIRS}
        )


ADD_SUBDIRECTORY(Source) #Needs to come first since Samples depend on it

# Use -DLIBTCOD_SAMPLES=OFF if you want do disable the building of the Samples (eg. library API changes).
IF (DORYEN_SAMPLES)
    ADD_SUBDIRECTORY(Samples/CPP)
    ADD_SUBDIRECTORY(Samples/Frost) # Frost effect demo
    ADD_SUBDIRECTORY(Samples/Navier) # Navier demo
    ADD_SUBDIRECTORY(Samples/Rad) # Radiosity demo
    ADD_SUBDIRECTORY(Samples/Ripples) # Water ripples demo
    ADD_SUBDIRECTORY(Samples/Weather) # Weather + Day/Night demo
    ADD_SUBDIRECTORY(Samples/Worldgen) # WorldGen demo
ENDIF ()

IF (DORYEN_TESTS)
    ADD_SUBDIRECTORY(Test)
ENDIF ()