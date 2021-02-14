# Support to Content Package Manager
INCLUDE(CMakeModules/CPM.cmake)

# Added Dependency for Execute Test
CPMADDPACKAGE(
        NAME doctest
        GITHUB_REPOSITORY onqtam/doctest
        GIT_TAG 2.4.5
)

# The order of definitions is important, The Unit directory need the definition
# provided for Hack directory.
ADD_SUBDIRECTORY(Hack)
ADD_SUBDIRECTORY(Unit)
ADD_SUBDIRECTORY(Overflow)
