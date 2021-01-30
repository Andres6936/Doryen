#[[ This sets an header-only project.

Don't worry, no actual binaries will be generated if you should try and build it.
The command <SET_TARGET_PROPERTIES> is there because CMake will otherwise complain
that it cannot infer the target language from .hpp files only.
]]
ADD_LIBRARY(Doryen.Test.Hack.Console INTERFACE)
ADD_LIBRARY(Doryen::Test::Hack::Console ALIAS Doryen.Test.Hack.Console)
# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
TARGET_INCLUDE_DIRECTORIES(Doryen.Test.Hack.Console INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>)
# Support to compiler on mode C++ 17 standard.
TARGET_COMPILE_FEATURES(Doryen.Test.Hack.Console INTERFACE cxx_std_17)
TARGET_LINK_LIBRARIES(Doryen.Test.Hack.Console INTERFACE Doryen::Framework)