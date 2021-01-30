ENABLE_TESTING()

MACRO(ADD_DORYEN_TEST _NAME_TARGET _NAME_FILE)
    ADD_EXECUTABLE(${_NAME_TARGET} ${_NAME_FILE})
    SET_TARGET_PROPERTIES(${_NAME_TARGET} PROPERTIES CXX_STANDARD 17)
    TARGET_LINK_LIBRARIES(${_NAME_TARGET} PRIVATE Doryen::Framework)
    TARGET_LINK_LIBRARIES(${_NAME_TARGET} PRIVATE doctest)
    ADD_TEST(NAME ${_NAME_TARGET} COMMAND ${_NAME_TARGET})
ENDMACRO()

#[[ For the tests that require use the class Console, is need move the font to
use, if on the other hand not is specified the font, the console throw an
exception, preventing the test from begin performed in this. ]]#
MACRO(ADD_DORYEN_TEST_WITH_RESOURCE _NAME_TARGET _NAME_FILE _PATH_RESOURCE)
    ADD_EXECUTABLE(${_NAME_TARGET} ${_NAME_FILE})
    SET_TARGET_PROPERTIES(${_NAME_TARGET} PROPERTIES CXX_STANDARD 17)
    TARGET_LINK_LIBRARIES(${_NAME_TARGET} PRIVATE Doryen::Framework)
    TARGET_LINK_LIBRARIES(${_NAME_TARGET} PRIVATE doctest)
    ADD_TEST(NAME ${_NAME_TARGET} COMMAND ${_NAME_TARGET})
    FILE(COPY ${_PATH_RESOURCE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDMACRO()

ADD_DORYEN_TEST(Doryen.Test.Unit.Cell TestCell.cpp)
ADD_DORYEN_TEST(Doryen.Test.Unit.Size TestSize.cpp)
ADD_DORYEN_TEST(Doryen.Test.Unit.Color TestColor.cpp)
ADD_DORYEN_TEST(Doryen.Test.Unit.Mipmap TestMipmap.cpp)
ADD_DORYEN_TEST(Doryen.Test.Unit.Point2D TestPoint2D.cpp)

# Moved the font to use by the console in this test. [Not need be Terminal.png, can be any other font].
ADD_DORYEN_TEST_WITH_RESOURCE(Doryen.Test.Unit.Console TestConsole.cpp ${DORYEN_ROOT_DIR}/Terminal.png)
TARGET_LINK_LIBRARIES(Doryen.Test.Unit.Console PRIVATE Doryen::Test::Hack::Console)