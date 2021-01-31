# The OBJECT's can be used to compile the sources in the list given to add_library
# to object files, but then neither archiving them into a static library nor
# linking them into a shared object. The use of object libraries is
# particularly useful if one needs to create both static and shared libraries
# in one go.

# Properties: OBJECT
ADD_LIBRARY(Doryen.Algorithms.Noise.Object OBJECT
        Noise/Simplex.cpp
        Noise/Wavelet.cpp
        Noise/Perlin.cpp
        Noise/Noise.cpp
        Noise/FBM.cpp
        )

SET_TARGET_PROPERTIES(Doryen.Algorithms.Noise.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Algorithms.Noise.Object PRIVATE ${DORYEN_ROOT_DIR}/Include/)

# Properties: OBJECT
ADD_LIBRARY(Doryen.Algorithms.Heightmap.Object OBJECT
        Generation/Heightmap.cpp
        )

SET_TARGET_PROPERTIES(Doryen.Algorithms.Heightmap.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Algorithms.Heightmap.Object PRIVATE ${DORYEN_ROOT_DIR}/Include/)

IF (DORYEN_BUILD_SHARED_LIBS)

    # The POSITION_INDEPENDENT_CODE property determines whether position independent executables or
    # shared libraries will be created. This property is True by default for SHARED and MODULE library
    # targets and False otherwise.

    # Property: SHARED, POSITION_INDEPENDENT_CODE
    ADD_LIBRARY(Doryen.Algorithms.Noise SHARED $<TARGET_OBJECTS:Doryen.Algorithms.Noise.Object>)
    ADD_LIBRARY(Doryen.Algorithms.Heightmap SHARED $<TARGET_OBJECTS:Doryen.Algorithms.Heightmap.Object>)

ELSE ()

    ADD_LIBRARY(Doryen.Algorithms.Noise STATIC $<TARGET_OBJECTS:Doryen.Algorithms.Noise.Object>)
    ADD_LIBRARY(Doryen.Algorithms.Heightmap STATIC $<TARGET_OBJECTS:Doryen.Algorithms.Heightmap.Object>)

ENDIF ()

ADD_LIBRARY(Doryen::Algorithms::Noise ALIAS Doryen.Algorithms.Noise)
# The generation of Noise depend of the generation of random numbers.
TARGET_LINK_LIBRARIES(Doryen.Algorithms.Noise PRIVATE Doryen::Random)

ADD_LIBRARY(Doryen::Algorithms::Heightmap ALIAS Doryen.Algorithms.Heightmap)
# The generation of Heightmap depend of the generation of Noise
# Thus, Heightmap Library depend of Noise Library
# The relation of this two libraries is public
TARGET_LINK_LIBRARIES(Doryen.Algorithms.Heightmap PUBLIC Doryen::Algorithms::Noise)

