ADD_EXECUTABLE(Worldgen
        Main.hpp Main.cpp UtilWorldgen.hpp UtilWorldgen.cpp MapData.cpp MapData.hpp)

SET_TARGET_PROPERTIES(Worldgen PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Worldgen PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Worldgen PRIVATE Doryen.Core.Cpp)