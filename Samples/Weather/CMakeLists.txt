ADD_EXECUTABLE(Weather
        Main.hpp Main.cpp UtilWeather.hpp UtilWeather.cpp)

SET_TARGET_PROPERTIES(Weather PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Weather PRIVATE Doryen::Algorithms::Heightmap)
TARGET_LINK_LIBRARIES(Weather PRIVATE Doryen::Framework)
TARGET_LINK_LIBRARIES(Weather PRIVATE Doryen::Random)

# Moving directory with resources needed
FILE(COPY ../../Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data)
FILE(COPY ../../Terminal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)