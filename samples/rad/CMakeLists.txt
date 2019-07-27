SET(LIBTCOD_RADIOSITY_SRC main.cpp bsp_helper.cpp rad_shader.cpp rad_shader_photon.cpp rad_shader_standard.cpp)

ADD_EXECUTABLE(rad ${LIBTCOD_RADIOSITY_SRC})
TARGET_LINK_LIBRARIES(rad ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})
