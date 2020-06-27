ADD_EXECUTABLE(Weather
        Main.hpp Main.cpp UtilWeather.hpp UtilWeather.cpp)

SET_TARGET_PROPERTIES(Weather PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Weather PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Weather PRIVATE Doryen.Core.Cpp)
