ENABLE_TESTING()

ADD_EXECUTABLE(Doryen.Test.Overflow.Color TestColor.cpp)
TARGET_LINK_LIBRARIES(Doryen.Test.Overflow.Color PRIVATE Doryen.Core.Cpp)
TARGET_LINK_LIBRARIES(Doryen.Test.Overflow.Color PRIVATE doctest)
ADD_TEST(NAME Doryen.Test.Overflow.Color COMMAND Doryen.Test.Overflow.Color)