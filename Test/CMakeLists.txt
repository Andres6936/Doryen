# Support to Content Package Manager
INCLUDE(CMakeModules/CPM.cmake)

# Added Dependency for Execute Test
CPMADDPACKAGE(
        NAME doctest
        GITHUB_REPOSITORY onqtam/doctest
        GIT_TAG 2.4.0
)

ADD_SUBDIRECTORY(Hack)
ADD_SUBDIRECTORY(Unit)
ADD_SUBDIRECTORY(Overflow)
