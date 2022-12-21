cmake_minimum_required(VERSION 3.20)

Message("")
Message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
Message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
Message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
Message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
Message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
Message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
Message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

project(cube_puzzle VERSION 0.9)

# set cmake variables
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(CMAKE_XCODE_SCHEME_WORKING_DIRECTORY ./)

# find all source files
file(GLOB_RECURSE SOURCE_FILES src/*.*)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SOURCE_FILES})
Message("")
Message(STATUS "SOURCE entry point : " ${SOURCE_FILES})
Message("")

# set windows variables for library includes
IF (WIN32 OR MINGW)
    set(ENV{SDL2} dev_lib/SDL2-2.26.1/)
    set(ENV{SDL2_IMAGE} dev_lib/SDL2_image-2.6.2/)
    set(ENV{SDL2TTFDIR} dev_lib/SDL2_ttf-2.20.1/)
    set(ENV{SDL2_MIXER} dev_lib/SDL2_mixer-2.6.2/)
endif()
# import all necessary libraries
include(CMAKE_library.cmake)
# copy windows dlls into bin directory
include(CMAKE_copy_dev_dll.cmake)


# include generated header file
set(REAL_SRC_DIR ${CMAKE_SOURCE_DIR})
include(CMAKE_configure_package.cmake)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/generated
        )

# pre build scripts
## update architecture, version, git commit has, etc.
add_custom_command( TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -D REAL_SRC_DIR="${CMAKE_SOURCE_DIR}" -D CMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}" -D CMAKE_RUNTIME_OUTPUT_DIRECTORY="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" -D CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}" -P ${CMAKE_SOURCE_DIR}/CMAKE_configure_package.cmake
        )

# post build scripts

## zip program
add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -D CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}" -D CMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}" -P ${CMAKE_SOURCE_DIR}/CMAKE_package.cmake)