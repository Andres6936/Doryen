ADD_EXECUTABLE(Ripples
        Main.hpp Main.cpp UtilRipples.hpp UtilRipples.cpp)

SET_TARGET_PROPERTIES(Ripples PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Ripples PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Ripples PRIVATE Doryen.Core.Cpp)
