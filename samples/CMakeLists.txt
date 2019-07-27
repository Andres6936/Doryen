SET(LIBTCOD_SAMPLE_C_SRC samples_c.c)
SET(LIBTCOD_SAMPLE_CPP_SRC samples_cpp.cpp)
SET(LIBTCOD_HP_TOOL_SRC hmtool/operation.cpp hmtool/hmtool.cpp)

# samples_c.c needs SDL/SDL.h, so we need the Include dir
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
ADD_EXECUTABLE(samples_c ${LIBTCOD_SAMPLE_C_SRC})
TARGET_LINK_LIBRARIES(samples_c ${LIBTCOD_NAME})

ADD_EXECUTABLE(samples_cpp ${LIBTCOD_SAMPLE_CPP_SRC})
TARGET_LINK_LIBRARIES(samples_cpp ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})  #${LIBTCOD_CPP_NAME} is empty on non-linux platforms

ADD_EXECUTABLE(hmtool.bin ${LIBTCOD_HP_TOOL_SRC})
TARGET_LINK_LIBRARIES(hmtool.bin ${LIBTCOD_NAME} ${LIBTCOD_GUI_NAME})

