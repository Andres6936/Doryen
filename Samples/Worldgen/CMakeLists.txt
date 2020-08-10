ADD_EXECUTABLE(Worldgen
        Main.hpp Main.cpp UtilWorldgen.hpp UtilWorldgen.cpp MapData.cpp MapData.hpp)

SET_TARGET_PROPERTIES(Worldgen PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Worldgen PRIVATE Doryen::Framework)

# Moving directory with resources needed
FILE(COPY ../../Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data)
FILE(COPY ../../Terminal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)