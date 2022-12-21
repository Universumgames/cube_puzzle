#pragma once

// settings file containing all paths used in game

#define LEVELS_DIR "./asset/levels/"
#define TITLESCREEN_LEVEL_PATH LEVELS_DIR "title.screen"
#define TITLESCREEN_LEVEL_SOLUTION_PATH LEVELS_DIR "titlescreen.solution"

#define ROBOTO_FONT_FILEPATH "./asset/font/RobotoSlab-Bold.ttf"
#define ROBOTO_FONT_LIGHT_FILEPATH "./asset/font/Roboto/Roboto-Regular.ttf"

#define MUSIC_BACKGROUND_PATH "asset/audio/background.wav"
#define MUSIC_HIT_WALL_PATH "asset/audio/hit2.wav"
#define MUSIC_PRESSURE_PLATE_PRESSED "asset/audio/pressed.wav"
#define MUSIC_LEVEL_FINISHED "asset/audio/finish.wav"

#define ICON_PATH "./asset/graphic/mac1024_2.png"
// "./asset/graphic/mac1024.png"

#define PLAYER_SPRITE_SHEET_PATH "./asset/graphic/playerSheet.png"
#define ARROW_SEMICIRCLE_PATH "./asset/graphic/arroMini.png"
#define ARROW_STRAIGHT_PATH "./asset/graphic/rrowig.png"
#define CUBE_FIELD_PATH "./asset/graphic/AllTestTyle.png"
#define CUBE_OBJECT_PATH "./asset/graphic/AllTestTyle.png"
#define TITLESCREEEN_GAME_BANNER_PATH "./asset/graphic/titlescreen.png"

#define CUBE_SIDE_BACKGROUND_PATH(side) ("./asset/graphic/Cube" + std::to_string(side) + ".png").c_str()