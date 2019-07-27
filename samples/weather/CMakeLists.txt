SET(LIBTCOD_WEATHER_SRC main.hpp main.cpp util_weather.hpp util_weather.cpp)

ADD_EXECUTABLE(weather ${LIBTCOD_WEATHER_SRC})
TARGET_LINK_LIBRARIES(weather ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})
