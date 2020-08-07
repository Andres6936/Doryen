ADD_EXECUTABLE(SamplesCPP
        SamplesCPP.cpp SampleRenderer.cpp LineListener.cpp BspListener.cpp
        Functor/Bsp.cpp Functor/Color.cpp Functor/Fov.cpp Functor/Image.cpp
        Functor/Lines.cpp Functor/Mouse.cpp Functor/Noise.cpp
        Functor/Offscreen.cpp Functor/Path.cpp Functor/Sdl.cpp)

SET_TARGET_PROPERTIES(SamplesCPP PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(SamplesCPP PRIVATE Doryen::Core::Cpp)
