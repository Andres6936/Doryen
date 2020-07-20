ADD_EXECUTABLE(Rad
        Main.cpp BSPHelper.cpp Shader.cpp RadShaderPhoton.cpp RadShaderStandard.cpp)

SET_TARGET_PROPERTIES(Rad PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Rad PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Rad PRIVATE Doryen.Core.Cpp)
