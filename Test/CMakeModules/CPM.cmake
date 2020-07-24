# CPM.cmake - CMake's missing package manager
# ===========================================
# See https://github.com/TheLartians/CPM.cmake for usage and update instructions.
#
# MIT License
# -----------
#[[
  Copyright (c) 2019 Lars Melchior

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]]

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

SET(CURRENT_CPM_VERSION 0.27.1)

IF (CPM_DIRECTORY)
    IF (NOT CPM_DIRECTORY STREQUAL CMAKE_CURRENT_LIST_DIR)
        IF (CPM_VERSION VERSION_LESS CURRENT_CPM_VERSION)
            MESSAGE(AUTHOR_WARNING "${CPM_INDENT} \
A dependency is using a more recent CPM version (${CURRENT_CPM_VERSION}) than the current project (${CPM_VERSION}). \
It is recommended to upgrade CPM to the most recent version. \
See https://github.com/TheLartians/CPM.cmake for more information."
                    )
        ENDIF ()
        RETURN()
    ENDIF ()

    GET_PROPERTY(CPM_INITIALIZED GLOBAL "" PROPERTY CPM_INITIALIZED SET)
    IF (CPM_INITIALIZED)
        RETURN()
    ENDIF ()
ENDIF ()

SET_PROPERTY(GLOBAL PROPERTY CPM_INITIALIZED true)

OPTION(CPM_USE_LOCAL_PACKAGES "Always try to use `find_package` to get dependencies" $ENV{CPM_USE_LOCAL_PACKAGES})
OPTION(CPM_LOCAL_PACKAGES_ONLY "Only use `find_package` to get dependencies" $ENV{CPM_LOCAL_PACKAGES_ONLY})
OPTION(CPM_DOWNLOAD_ALL "Always download dependencies from source" $ENV{CPM_DOWNLOAD_ALL})
OPTION(CPM_DONT_UPDATE_MODULE_PATH "Don't update the module path to allow using find_package" $ENV{CPM_DONT_UPDATE_MODULE_PATH})
OPTION(CPM_DONT_CREATE_PACKAGE_LOCK "Don't create a package lock file in the binary path" $ENV{CPM_DONT_CREATE_PACKAGE_LOCK})
OPTION(CPM_INCLUDE_ALL_IN_PACKAGE_LOCK "Add all packages added through CPM.cmake to the package lock" $ENV{CPM_INCLUDE_ALL_IN_PACKAGE_LOCK})

SET(CPM_VERSION ${CURRENT_CPM_VERSION} CACHE INTERNAL "")
SET(CPM_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")
SET(CPM_FILE ${CMAKE_CURRENT_LIST_FILE} CACHE INTERNAL "")
SET(CPM_PACKAGES "" CACHE INTERNAL "")
SET(CPM_DRY_RUN OFF CACHE INTERNAL "Don't download or configure dependencies (for testing)")

IF (DEFINED ENV{CPM_SOURCE_CACHE})
    SET(CPM_SOURCE_CACHE_DEFAULT $ENV{CPM_SOURCE_CACHE})
ELSE ()
    SET(CPM_SOURCE_CACHE_DEFAULT OFF)
ENDIF ()

SET(CPM_SOURCE_CACHE ${CPM_SOURCE_CACHE_DEFAULT} CACHE PATH "Directory to downlaod CPM dependencies")

IF (NOT CPM_DONT_UPDATE_MODULE_PATH)
    SET(CPM_MODULE_PATH "${CMAKE_BINARY_DIR}/CPM_modules" CACHE INTERNAL "")
    # remove old modules
    FILE(REMOVE_RECURSE ${CPM_MODULE_PATH})
    FILE(MAKE_DIRECTORY ${CPM_MODULE_PATH})
    # locally added CPM modules should override global packages
    SET(CMAKE_MODULE_PATH "${CPM_MODULE_PATH};${CMAKE_MODULE_PATH}")
ENDIF ()

IF (NOT CPM_DONT_CREATE_PACKAGE_LOCK)
    SET(CPM_PACKAGE_LOCK_FILE "${CMAKE_BINARY_DIR}/cpm-package-lock.cmake" CACHE INTERNAL "")
    FILE(WRITE ${CPM_PACKAGE_LOCK_FILE} "# CPM Package Lock\n# This file should be committed to version control\n\n")
ENDIF ()

INCLUDE(FetchContent)
INCLUDE(CMakeParseArguments)

# Initialize logging prefix
IF (NOT CPM_INDENT)
    SET(CPM_INDENT "CPM:")
ENDIF ()

FUNCTION(CPM_FIND_PACKAGE NAME VERSION)
    STRING(REPLACE " " ";" EXTRA_ARGS "${ARGN}")
    FIND_PACKAGE(${NAME} ${VERSION} ${EXTRA_ARGS} QUIET)
    IF (${CPM_ARGS_NAME}_FOUND)
        MESSAGE(STATUS "${CPM_INDENT} using local package ${CPM_ARGS_NAME}@${VERSION}")
        CPMREGISTERPACKAGE(${CPM_ARGS_NAME} "${VERSION}")
        SET(CPM_PACKAGE_FOUND YES PARENT_SCOPE)
    ELSE ()
        SET(CPM_PACKAGE_FOUND NO PARENT_SCOPE)
    ENDIF ()
ENDFUNCTION()

# Create a custom FindXXX.cmake module for a CPM package
# This prevents `find_package(NAME)` from finding the system library
FUNCTION(CPMCREATEMODULEFILE Name)
    IF (NOT CPM_DONT_UPDATE_MODULE_PATH)
        # erase any previous modules
        FILE(WRITE ${CPM_MODULE_PATH}/Find${Name}.cmake "include(${CPM_FILE})\n${ARGN}\nset(${Name}_FOUND TRUE)")
    ENDIF ()
ENDFUNCTION()

# Find a package locally or fallback to CPMAddPackage
FUNCTION(CPMFINDPACKAGE)
    SET(oneValueArgs
            NAME
            VERSION
            FIND_PACKAGE_ARGUMENTS
            )

    CMAKE_PARSE_ARGUMENTS(CPM_ARGS "" "${oneValueArgs}" "" ${ARGN})

    IF (NOT DEFINED CPM_ARGS_VERSION)
        IF (DEFINED CPM_ARGS_GIT_TAG)
            CPM_GET_VERSION_FROM_GIT_TAG("${CPM_ARGS_GIT_TAG}" CPM_ARGS_VERSION)
        ENDIF ()
    ENDIF ()

    IF (CPM_DOWNLOAD_ALL)
        CPMADDPACKAGE(${ARGN})
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
        RETURN()
    ENDIF ()

    CPMCHECKIFPACKAGEALREADYADDED(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" "${CPM_ARGS_OPTIONS}")
    IF (CPM_PACKAGE_ALREADY_ADDED)
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
        RETURN()
    ENDIF ()

    CPM_FIND_PACKAGE(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" ${CPM_ARGS_FIND_PACKAGE_ARGUMENTS})

    IF (NOT CPM_PACKAGE_FOUND)
        CPMADDPACKAGE(${ARGN})
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
    ENDIF ()

ENDFUNCTION()

# checks if a package has been added before
FUNCTION(CPMCHECKIFPACKAGEALREADYADDED CPM_ARGS_NAME CPM_ARGS_VERSION CPM_ARGS_OPTIONS)
    IF ("${CPM_ARGS_NAME}" IN_LIST CPM_PACKAGES)
        CPMGETPACKAGEVERSION(${CPM_ARGS_NAME} CPM_PACKAGE_VERSION)
        IF ("${CPM_PACKAGE_VERSION}" VERSION_LESS "${CPM_ARGS_VERSION}")
            MESSAGE(WARNING "${CPM_INDENT} requires a newer version of ${CPM_ARGS_NAME} (${CPM_ARGS_VERSION}) than currently included (${CPM_PACKAGE_VERSION}).")
        ENDIF ()
        IF (CPM_ARGS_OPTIONS)
            FOREACH (OPTION ${CPM_ARGS_OPTIONS})
                CPM_PARSE_OPTION(${OPTION})
                IF (NOT "${${OPTION_KEY}}" STREQUAL "${OPTION_VALUE}")
                    MESSAGE(WARNING "${CPM_INDENT} ignoring package option for ${CPM_ARGS_NAME}: ${OPTION_KEY} = ${OPTION_VALUE} (${${OPTION_KEY}})")
                ENDIF ()
            ENDFOREACH ()
        ENDIF ()
        CPM_GET_FETCH_PROPERTIES(${CPM_ARGS_NAME})
        SET(${CPM_ARGS_NAME}_ADDED NO)
        SET(CPM_PACKAGE_ALREADY_ADDED YES PARENT_SCOPE)
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
    ELSE ()
        SET(CPM_PACKAGE_ALREADY_ADDED NO PARENT_SCOPE)
    ENDIF ()
ENDFUNCTION()

# Download and add a package from source
FUNCTION(CPMADDPACKAGE)

    SET(oneValueArgs
            NAME
            FORCE
            VERSION
            GIT_TAG
            DOWNLOAD_ONLY
            GITHUB_REPOSITORY
            GITLAB_REPOSITORY
            GIT_REPOSITORY
            SOURCE_DIR
            DOWNLOAD_COMMAND
            FIND_PACKAGE_ARGUMENTS
            NO_CACHE
            GIT_SHALLOW
            )

    SET(multiValueArgs
            OPTIONS
            )

    CMAKE_PARSE_ARGUMENTS(CPM_ARGS "" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    # Set default values for arguments

    IF (NOT DEFINED CPM_ARGS_VERSION)
        IF (DEFINED CPM_ARGS_GIT_TAG)
            CPM_GET_VERSION_FROM_GIT_TAG("${CPM_ARGS_GIT_TAG}" CPM_ARGS_VERSION)
        ENDIF ()
    ENDIF ()

    IF (CPM_ARGS_DOWNLOAD_ONLY)
        SET(DOWNLOAD_ONLY ${CPM_ARGS_DOWNLOAD_ONLY})
    ELSE ()
        SET(DOWNLOAD_ONLY NO)
    ENDIF ()

    IF (DEFINED CPM_ARGS_GITHUB_REPOSITORY)
        SET(CPM_ARGS_GIT_REPOSITORY "https://github.com/${CPM_ARGS_GITHUB_REPOSITORY}.git")
    ENDIF ()

    IF (DEFINED CPM_ARGS_GITLAB_REPOSITORY)
        LIST(CPM_ARGS_GIT_REPOSITORY "https://gitlab.com/${CPM_ARGS_GITLAB_REPOSITORY}.git")
    ENDIF ()

    IF (DEFINED CPM_ARGS_GIT_REPOSITORY)
        LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_REPOSITORY ${CPM_ARGS_GIT_REPOSITORY})
        IF (NOT DEFINED CPM_ARGS_GIT_TAG)
            SET(CPM_ARGS_GIT_TAG v${CPM_ARGS_VERSION})
        ENDIF ()
    ENDIF ()

    IF (DEFINED CPM_ARGS_GIT_TAG)
        LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_TAG ${CPM_ARGS_GIT_TAG})
        # If GIT_SHALLOW is explicitly specified, honor the value.
        IF (DEFINED CPM_ARGS_GIT_SHALLOW)
            LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_SHALLOW ${CPM_ARGS_GIT_SHALLOW})
        ENDIF ()
    ENDIF ()

    # Check if package has been added before
    CPMCHECKIFPACKAGEALREADYADDED(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" "${CPM_ARGS_OPTIONS}")
    IF (CPM_PACKAGE_ALREADY_ADDED)
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
        RETURN()
    ENDIF ()

    # Check for manual overrides
    IF (NOT CPM_ARGS_FORCE AND NOT "${CPM_${CPM_ARGS_NAME}_SOURCE}" STREQUAL "")
        SET(PACKAGE_SOURCE ${CPM_${CPM_ARGS_NAME}_SOURCE})
        SET(CPM_${CPM_ARGS_NAME}_SOURCE "")
        CPMADDPACKAGE(
                NAME ${CPM_ARGS_NAME}
                SOURCE_DIR ${PACKAGE_SOURCE}
                FORCE True
        )
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
        RETURN()
    ENDIF ()

    # Check for available declaration
    IF (NOT CPM_ARGS_FORCE AND NOT "${CPM_DECLARATION_${CPM_ARGS_NAME}}" STREQUAL "")
        SET(declaration ${CPM_DECLARATION_${CPM_ARGS_NAME}})
        SET(CPM_DECLARATION_${CPM_ARGS_NAME} "")
        CPMADDPACKAGE(${declaration})
        CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
        # checking again to ensure version and option compatibility
        CPMCHECKIFPACKAGEALREADYADDED(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" "${CPM_ARGS_OPTIONS}")
        RETURN()
    ENDIF ()

    IF (CPM_USE_LOCAL_PACKAGES OR CPM_LOCAL_PACKAGES_ONLY)
        CPM_FIND_PACKAGE(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" ${CPM_ARGS_FIND_PACKAGE_ARGUMENTS})

        IF (CPM_PACKAGE_FOUND)
            CPM_EXPORT_VARIABLES(${CPM_ARGS_NAME})
            RETURN()
        ENDIF ()

        IF (CPM_LOCAL_PACKAGES_ONLY)
            MESSAGE(SEND_ERROR "CPM: ${CPM_ARGS_NAME} not found via find_package(${CPM_ARGS_NAME} ${CPM_ARGS_VERSION})")
        ENDIF ()
    ENDIF ()

    CPMREGISTERPACKAGE("${CPM_ARGS_NAME}" "${CPM_ARGS_VERSION}")

    IF (CPM_ARGS_OPTIONS)
        FOREACH (OPTION ${CPM_ARGS_OPTIONS})
            CPM_PARSE_OPTION(${OPTION})
            SET(${OPTION_KEY} ${OPTION_VALUE} CACHE INTERNAL "")
        ENDFOREACH ()
    ENDIF ()

    IF (DEFINED CPM_ARGS_GIT_TAG)
        SET(PACKAGE_INFO "${CPM_ARGS_GIT_TAG}")
    ELSEIF (DEFINED CPM_ARGS_SOURCE_DIR)
        SET(PACKAGE_INFO "${CPM_ARGS_SOURCE_DIR}")
    ELSE ()
        SET(PACKAGE_INFO "${CPM_ARGS_VERSION}")
    ENDIF ()

    IF (DEFINED CPM_ARGS_DOWNLOAD_COMMAND)
        LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS DOWNLOAD_COMMAND ${CPM_ARGS_DOWNLOAD_COMMAND})
    ELSEIF (DEFINED CPM_ARGS_SOURCE_DIR)
        LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS SOURCE_DIR ${CPM_ARGS_SOURCE_DIR})
    ELSEIF (CPM_SOURCE_CACHE AND NOT CPM_ARGS_NO_CACHE)
        STRING(TOLOWER ${CPM_ARGS_NAME} lower_case_name)
        SET(origin_parameters ${CPM_ARGS_UNPARSED_ARGUMENTS})
        LIST(SORT origin_parameters)
        STRING(SHA1 origin_hash "${origin_parameters}")
        SET(download_directory ${CPM_SOURCE_CACHE}/${lower_case_name}/${origin_hash})
        LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS SOURCE_DIR ${download_directory})
        IF (EXISTS ${download_directory})
            # disable the download command to allow offline builds
            LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS DOWNLOAD_COMMAND "${CMAKE_COMMAND}")
            SET(PACKAGE_INFO "${download_directory}")
        ELSE ()
            # Enable shallow clone when GIT_TAG is not a commit hash.
            # Our guess may not be accurate, but it should guarantee no commit hash get mis-detected.
            IF (NOT DEFINED CPM_ARGS_GIT_SHALLOW)
                CPM_IS_GIT_TAG_COMMIT_HASH("${CPM_ARGS_GIT_TAG}" IS_HASH)
                IF (NOT ${IS_HASH})
                    LIST(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_SHALLOW TRUE)
                ENDIF ()
            ENDIF ()

            # remove timestamps so CMake will re-download the dependency
            FILE(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/_deps/${lower_case_name}-subbuild)
            SET(PACKAGE_INFO "${PACKAGE_INFO} -> ${download_directory}")
        ENDIF ()
    ENDIF ()

    CPMCREATEMODULEFILE(${CPM_ARGS_NAME} "CPMAddPackage(${ARGN})")

    IF (CPM_PACKAGE_LOCK_ENABLED)
        IF ((CPM_ARGS_VERSION AND NOT CPM_ARGS_SOURCE_DIR) OR CPM_INCLUDE_ALL_IN_PACKAGE_LOCK)
            CPM_ADD_TO_PACKAGE_LOCK(${CPM_ARGS_NAME} "${ARGN}")
        ELSEIF (CPM_ARGS_SOURCE_DIR)
            CPM_ADD_COMMENT_TO_PACKAGE_LOCK(${CPM_ARGS_NAME} "local directory")
        ELSE ()
            CPM_ADD_COMMENT_TO_PACKAGE_LOCK(${CPM_ARGS_NAME} "${ARGN}")
        ENDIF ()
    ENDIF ()

    CPM_DECLARE_FETCH("${CPM_ARGS_NAME}" "${CPM_ARGS_VERSION}" "${PACKAGE_INFO}" "${CPM_ARGS_UNPARSED_ARGUMENTS}")
    CPM_FETCH_PACKAGE("${CPM_ARGS_NAME}" "${DOWNLOAD_ONLY}")
    CPM_GET_FETCH_PROPERTIES("${CPM_ARGS_NAME}")

    SET(${CPM_ARGS_NAME}_ADDED YES)
    CPM_EXPORT_VARIABLES("${CPM_ARGS_NAME}")
ENDFUNCTION()

# Fetch a previously declared package
MACRO(CPMGETPACKAGE Name)
    IF (DEFINED "CPM_DECLARATION_${Name}")
        CPMADDPACKAGE(
                NAME ${Name}
        )
    ELSE ()
        MESSAGE(SEND_ERROR "Cannot retrieve package ${Name}: no declaration available")
    ENDIF ()
ENDMACRO()

# export variables available to the caller to the parent scope
# expects ${CPM_ARGS_NAME} to be set
MACRO(CPM_EXPORT_VARIABLES name)
    SET(${name}_SOURCE_DIR "${${name}_SOURCE_DIR}" PARENT_SCOPE)
    SET(${name}_BINARY_DIR "${${name}_BINARY_DIR}" PARENT_SCOPE)
    SET(${name}_ADDED "${${name}_ADDED}" PARENT_SCOPE)
ENDMACRO()

# declares a package, so that any call to CPMAddPackage for the
# package name will use these arguments instead.
# Previous declarations will not be overriden.
MACRO(CPMDECLAREPACKAGE Name)
    IF (NOT DEFINED "CPM_DECLARATION_${Name}")
        SET("CPM_DECLARATION_${Name}" "${ARGN}")
    ENDIF ()
ENDMACRO()

FUNCTION(CPM_ADD_TO_PACKAGE_LOCK Name)
    IF (NOT CPM_DONT_CREATE_PACKAGE_LOCK)
        FILE(APPEND ${CPM_PACKAGE_LOCK_FILE} "# ${Name}\nCPMDeclarePackage(${Name} \"${ARGN}\")\n")
    ENDIF ()
ENDFUNCTION()

FUNCTION(CPM_ADD_COMMENT_TO_PACKAGE_LOCK Name)
    IF (NOT CPM_DONT_CREATE_PACKAGE_LOCK)
        FILE(APPEND ${CPM_PACKAGE_LOCK_FILE} "# ${Name} (unversioned)\n# CPMDeclarePackage(${Name} \"${ARGN}\")\n")
    ENDIF ()
ENDFUNCTION()

# includes the package lock file if it exists and creates a target
# `cpm-write-package-lock` to update it
MACRO(CPMUSEPACKAGELOCK file)
    IF (NOT CPM_DONT_CREATE_PACKAGE_LOCK)
        GET_FILENAME_COMPONENT(CPM_ABSOLUTE_PACKAGE_LOCK_PATH ${file} ABSOLUTE)
        IF (EXISTS ${CPM_ABSOLUTE_PACKAGE_LOCK_PATH})
            INCLUDE(${CPM_ABSOLUTE_PACKAGE_LOCK_PATH})
        ENDIF ()
        IF (NOT TARGET cpm-update-package-lock)
            ADD_CUSTOM_TARGET(cpm-update-package-lock COMMAND ${CMAKE_COMMAND} -E copy ${CPM_PACKAGE_LOCK_FILE} ${CPM_ABSOLUTE_PACKAGE_LOCK_PATH})
        ENDIF ()
        SET(CPM_PACKAGE_LOCK_ENABLED true)
    ENDIF ()
ENDMACRO()

# registers a package that has been added to CPM
FUNCTION(CPMREGISTERPACKAGE PACKAGE VERSION)
    LIST(APPEND CPM_PACKAGES ${PACKAGE})
    SET(CPM_PACKAGES ${CPM_PACKAGES} CACHE INTERNAL "")
    SET("CPM_PACKAGE_${PACKAGE}_VERSION" ${VERSION} CACHE INTERNAL "")
ENDFUNCTION()

# retrieve the current version of the package to ${OUTPUT}
FUNCTION(CPMGETPACKAGEVERSION PACKAGE OUTPUT)
    SET(${OUTPUT} "${CPM_PACKAGE_${PACKAGE}_VERSION}" PARENT_SCOPE)
ENDFUNCTION()

# declares a package in FetchContent_Declare
FUNCTION(CPM_DECLARE_FETCH PACKAGE VERSION INFO)
    MESSAGE(STATUS "${CPM_INDENT} adding package ${PACKAGE}@${VERSION} (${INFO})")

    IF (${CPM_DRY_RUN})
        MESSAGE(STATUS "${CPM_INDENT} package not declared (dry run)")
        RETURN()
    ENDIF ()

    FETCHCONTENT_DECLARE(${PACKAGE}
            ${ARGN}
            )
ENDFUNCTION()

# returns properties for a package previously defined by cpm_declare_fetch
FUNCTION(CPM_GET_FETCH_PROPERTIES PACKAGE)
    IF (${CPM_DRY_RUN})
        RETURN()
    ENDIF ()
    FETCHCONTENT_GETPROPERTIES(${PACKAGE})
    STRING(TOLOWER ${PACKAGE} lpackage)
    SET(${PACKAGE}_SOURCE_DIR "${${lpackage}_SOURCE_DIR}" PARENT_SCOPE)
    SET(${PACKAGE}_BINARY_DIR "${${lpackage}_BINARY_DIR}" PARENT_SCOPE)
ENDFUNCTION()

# downloads a previously declared package via FetchContent
FUNCTION(CPM_FETCH_PACKAGE PACKAGE DOWNLOAD_ONLY)
    IF (${CPM_DRY_RUN})
        MESSAGE(STATUS "${CPM_INDENT} package ${PACKAGE} not fetched (dry run)")
        RETURN()
    ENDIF ()

    IF (DOWNLOAD_ONLY)
        FETCHCONTENT_GETPROPERTIES(${PACKAGE})
        IF (NOT ${PACKAGE}_POPULATED)
            FETCHCONTENT_POPULATE(${PACKAGE})
        ENDIF ()
    ELSE ()
        SET(CPM_OLD_INDENT "${CPM_INDENT}")
        SET(CPM_INDENT "${CPM_INDENT} ${PACKAGE}:")
        FETCHCONTENT_MAKEAVAILABLE(${PACKAGE})
        SET(CPM_INDENT "${CPM_OLD_INDENT}")
    ENDIF ()
ENDFUNCTION()

# splits a package option
FUNCTION(CPM_PARSE_OPTION OPTION)
    STRING(REGEX MATCH "^[^ ]+" OPTION_KEY ${OPTION})
    STRING(LENGTH ${OPTION} OPTION_LENGTH)
    STRING(LENGTH ${OPTION_KEY} OPTION_KEY_LENGTH)
    IF (OPTION_KEY_LENGTH STREQUAL OPTION_LENGTH)
        # no value for key provided, assume user wants to set option to "ON"
        SET(OPTION_VALUE "ON")
    ELSE ()
        MATH(EXPR OPTION_KEY_LENGTH "${OPTION_KEY_LENGTH}+1")
        STRING(SUBSTRING ${OPTION} "${OPTION_KEY_LENGTH}" "-1" OPTION_VALUE)
    ENDIF ()
    SET(OPTION_KEY "${OPTION_KEY}" PARENT_SCOPE)
    SET(OPTION_VALUE "${OPTION_VALUE}" PARENT_SCOPE)
ENDFUNCTION()

# guesses the package version from a git tag
FUNCTION(CPM_GET_VERSION_FROM_GIT_TAG GIT_TAG RESULT)
    STRING(LENGTH ${GIT_TAG} length)
    IF (length EQUAL 40)
        # GIT_TAG is probably a git hash
        SET(${RESULT} 0 PARENT_SCOPE)
    ELSE ()
        STRING(REGEX MATCH "v?([0123456789.]*).*" _ ${GIT_TAG})
        SET(${RESULT} ${CMAKE_MATCH_1} PARENT_SCOPE)
    ENDIF ()
ENDFUNCTION()

# guesses if the git tag is a commit hash or an actual tag or a branch nane.
FUNCTION(CPM_IS_GIT_TAG_COMMIT_HASH GIT_TAG RESULT)
    STRING(LENGTH "${GIT_TAG}" length)
    # full hash has 40 characters, and short hash has at least 7 characters.
    IF (length LESS 7 OR length GREATER 40)
        SET(${RESULT} 0 PARENT_SCOPE)
    ELSE ()
        IF (${GIT_TAG} MATCHES "^[a-fA-F0-9]+$")
            SET(${RESULT} 1 PARENT_SCOPE)
        ELSE ()
            SET(${RESULT} 0 PARENT_SCOPE)
        ENDIF ()
    ENDIF ()
ENDFUNCTION()
