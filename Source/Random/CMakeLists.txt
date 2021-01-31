# The OBJECT's can be used to compile the sources in the list given to add_library
# to object files, but then neither archiving them into a static library nor
# linking them into a shared object. The use of object libraries is
# particularly useful if one needs to create both static and shared libraries
# in one go.

# Properties: OBJECT
ADD_LIBRARY(Doryen.Random.Object OBJECT Number.cpp)
SET_TARGET_PROPERTIES(Doryen.Random.Object PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Doryen.Random.Object PRIVATE ${DORYEN_ROOT_DIR}/Include/)

IF (DORYEN_SHARED_LIBRARIES)

    # The POSITION_INDEPENDENT_CODE property determines whether position independent executables or
    # shared libraries will be created. This property is True by default for SHARED and MODULE library
    # targets and False otherwise.

    # Property: SHARED, POSITION_INDEPENDENT_CODE
    ADD_LIBRARY(Doryen.Random SHARED $<TARGET_OBJECTS:Doryen.Random.Object>)

ELSE ()

    ADD_LIBRARY(Doryen.Random STATIC $<TARGET_OBJECTS:Doryen.Random.Object>)

ENDIF ()

ADD_LIBRARY(Doryen::Random ALIAS Doryen.Random)