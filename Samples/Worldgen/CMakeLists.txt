SET(LIBTCOD_WORLDGEN_SRC Main.hpp Main.cpp UtilWorldgen.hpp UtilWorldgen.cpp MapData.cpp MapData.hpp)

ADD_EXECUTABLE(Worldgen ${LIBTCOD_WORLDGEN_SRC})
TARGET_LINK_LIBRARIES(Worldgen ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})
