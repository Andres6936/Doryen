ADD_LIBRARY(Doryen.Algorithms.Noise.Object OBJECT
        Noise/Simplex.cpp
        Noise/Wavelet.cpp
        Noise/Perlin.cpp
        Noise/Noise.cpp
        Noise/FBM.cpp
        )

SET_TARGET_PROPERTIES(Doryen.Algorithms.Noise.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Algorithms.Noise.Object
        PUBLIC
        $<BUILD_INTERFACE:${DORYEN_ROOT_DIR}/Include/>
        $<INSTALL_INTERFACE:${DORYEN_ROOT_DIR}/Include/>
        )


ADD_LIBRARY(Doryen.Algorithms.Heightmap.Object OBJECT
        Generation/Heightmap.cpp
        )

SET_TARGET_PROPERTIES(Doryen.Algorithms.Heightmap.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Algorithms.Heightmap.Object
        PUBLIC
        $<BUILD_INTERFACE:${DORYEN_ROOT_DIR}/Include/>
        $<INSTALL_INTERFACE:${DORYEN_ROOT_DIR}/Include/>
        )

IF (DORYEN_SHARED_LIBRARIES)

    ADD_LIBRARY(Doryen.Algorithms.Noise SHARED $<TARGET_OBJECTS:Doryen.Algorithms.Noise.Object>)
    ADD_LIBRARY(Doryen.Algorithms.Heightmap SHARED $<TARGET_OBJECTS:Doryen.Algorithms.Heightmap.Object>)

ELSE ()

    ADD_LIBRARY(Doryen.Algorithms.Noise STATIC $<TARGET_OBJECTS:Doryen.Algorithms.Noise.Object>)
    ADD_LIBRARY(Doryen.Algorithms.Heightmap STATIC $<TARGET_OBJECTS:Doryen.Algorithms.Heightmap.Object>)

ENDIF ()

ADD_LIBRARY(Doryen::Algorithms::Noise ALIAS Doryen.Algorithms.Noise)
ADD_LIBRARY(Doryen::Algorithms::Heightmap ALIAS Doryen.Algorithms.Heightmap)
# The generation of Heightmap depend of the generation of Noise
# Thus, Heightmap Library depend of Noise Library
# The relation of this two libraries is public
TARGET_LINK_LIBRARIES(Doryen.Algorithms.Heightmap PUBLIC Doryen::Algorithms::Noise)

