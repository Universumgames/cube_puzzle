set(SDL2_INCLUDE_DIR ./SDL2-2.24.2/include)
set(SDL2_LIB_DIR ./SDL2-2.24.2/lib)

set(SDL2_TTF_INCLUDE_DIR ./SDL2_ttf-2.20.1/include)
set(SDL2_TTF_LIB_DIR ./SDL2_ttf-2.20.1/lib)

set(SDL2_IMAGE_INCLUDE_DIR ./SDL2_image-2.6.2/include)
set(SDL2_IMAGE_LIB_DIR ./SDL2_image-2.6.2/lib)

set(SDL2_MIXER_INCLUDE_DIR ./SDL2_mixer-2.6.2/include)
set(SDL2_MIXER_LIB_DIR ./SDL2_mixer-2.6.2/lib)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

include_directories(${SDL2_TTF_INCLUDE_DIR})
link_directories(${SDL2_TTF_LIB_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})
link_directories(${SDL2_IMAGE_LIB_DIR})

include_directories(${SDL2_MIXER_INCLUDE_DIR})
link_directories(${SDL2_MIXER_LIB_DIR})

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2main SDL2_ttf SDL2_image SDL2_mixer)

file(COPY asset DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})