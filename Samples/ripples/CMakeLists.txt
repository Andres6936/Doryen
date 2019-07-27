SET(LIBTCOD_RIPPLES_SRC main.hpp main.cpp util_ripples.hpp util_ripples.cpp)

ADD_EXECUTABLE(ripples ${LIBTCOD_RIPPLES_SRC})
TARGET_LINK_LIBRARIES(ripples ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})
