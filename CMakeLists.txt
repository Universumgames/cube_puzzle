cmake_minimum_required(VERSION 3.17)

Message("")
Message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
Message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
Message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
Message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
Message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
Message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
Message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

project(cube_puzzle)
# ------- Set Vars ------- #
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(SOURCE_FILES src/main.cpp src/global.hpp src/gamebase.hpp src/gamebase.cpp src/recthelper.hpp src/CubeGame.cpp src/CubeGame.hpp src/States/LevelSelector.cpp src/States/LevelSelector.hpp src/States/Level.cpp src/States/Level.hpp src/GameObjects/GameObject.cpp src/GameObjects/GameObject.hpp src/GameObjects/Player.cpp src/GameObjects/Player.hpp src/GameObjects/CubeMap.cpp src/GameObjects/CubeMap.hpp src/GameObjects/WorldMap.cpp src/GameObjects/WorldMap.hpp src/GameObjects/CubeField.cpp src/GameObjects/CubeField.hpp src/GameObjects/WorldField.hpp)

Message("")
Message(STATUS "SOURCE entry point : " ${SOURCE_FILES})
Message("")

# ------- End Set Vars --- #

# ------- Finds ---------- #
IF (WIN32 OR MINGW)
    set(SDL2_INCLUDE_DIR C:/libs/SDL2-2.24.0/include)
    set(SDL2_LIB_DIR C:/libs/SDL2-2.24.0/lib/x64)

    set(SDL2_TTF_INCLUDE_DIR C:/libs/SDL2_ttf-2.20.1/include)
    set(SDL2_TTF_LIB_DIR C:/libs/SDL2_ttf-2.20.1/lib/x64)

    set(SDL2_IMAGE_INCLUDE_DIR C:/libs/SDL2_image-2.6.2/x86_64-w64-mingw32/include)
    set(SDL2_IMAGE_LIB_DIR C:/libs/SDL2_image-2.6.2/x86_64-w64-mingw32/lib)

    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIB_DIR})

    include_directories(${SDL2_TTF_INCLUDE_DIR})
    link_directories(${SDL2_TTF_LIB_DIR})

    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    link_directories(${SDL2_IMAGE_LIB_DIR})

    ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf SDL2_image)

else ()
    ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

    FIND_PACKAGE(SDL2 REQUIRED)
    Message("")
    Message(STATUS "FINDING SDL2")
    IF (${SDL2_FOUND})
        Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message(STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
        Message(STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
    ELSE ()
        Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message(FATAL_ERROR "SDL2 NOT FOUND")
    ENDIF ()

    FIND_PACKAGE(SDL2_IMAGE)
    Message("")
    Message(STATUS "FINDING SDL2_IMAGE")
    Message(STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
    IF (${SDL2_IMAGE_FOUND})
        Message(STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
        Message(STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
    ELSE ()
        Message(FATAL_ERROR "SDL2_IMAGE NOT FOUND")
    ENDIF ()

    FIND_PACKAGE(SDL2TTF)
    Message("")
    Message(STATUS "FINDING SDL2TTF")
    Message(STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND})
    IF (${SDL2TTF_FOUND})
        Message(STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
        Message(STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
    ELSE ()
        Message(FATAL_ERROR "SDL2TTF NOT FOUND")
    ENDIF ()
    Message("")

    FIND_PACKAGE(SDL2Mixer)

    Message(STATUS "FINDING SDL2Mixer")
    Message(STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND})

    Message("")

    file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY asset DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # ------- End Finds ------ #

    # ------- Inc & Link ---- #

    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY})

    # ------- End ----------- #
ENDIF ()