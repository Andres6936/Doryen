SET(LIBTCOD_SAMPLE_CPP_SRC SamplesCPP.cpp SampleRenderer.cpp LineListener.cpp)

ADD_EXECUTABLE(SamplesCPP ${LIBTCOD_SAMPLE_CPP_SRC})
TARGET_LINK_LIBRARIES(SamplesCPP ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})  #${LIBTCOD_CPP_NAME} is empty on non-linux platforms

