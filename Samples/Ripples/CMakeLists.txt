ADD_EXECUTABLE(Ripples
        Main.hpp Main.cpp UtilRipples.hpp UtilRipples.cpp)

SET_TARGET_PROPERTIES(Ripples PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Ripples PRIVATE Doryen::Framework)
TARGET_LINK_LIBRARIES(Ripples PRIVATE Doryen::Algorithms::Noise)
TARGET_LINK_LIBRARIES(Ripples PRIVATE Doryen::Algorithms::Heightmap)

# Moving directory with resources needed
FILE(COPY ../../Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data)
FILE(COPY ../../Terminal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)