SET(LIBTCOD_WORLDGEN_SRC main.hpp main.cpp util_worldgen.hpp util_worldgen.cpp)

ADD_EXECUTABLE(worldgen ${LIBTCOD_WORLDGEN_SRC})
TARGET_LINK_LIBRARIES(worldgen ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})
