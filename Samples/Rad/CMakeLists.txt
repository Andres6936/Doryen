ADD_EXECUTABLE(Rad
        Main.cpp Dungeon.cpp Shader/Interface/Shader.cpp Shader/PhotonShader.cpp Shader/StandardShader.cpp)

SET_TARGET_PROPERTIES(Rad PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Rad PRIVATE Doryen.Core.C)
TARGET_LINK_LIBRARIES(Rad PRIVATE Doryen.Core.Cpp)
