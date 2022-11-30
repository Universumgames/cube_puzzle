#pragma once

#define LEVELS_DIR "./asset/levels/"

#define ROBOTO_FONT_FILEPATH "./asset/font/RobotoSlab-Bold.ttf"
#define ROBOTO_FONT_LIGHT_FILEPATH "./asset/font/Roboto/Roboto-Regular.ttf"

#define MUSIC_BACKGROUND_PATH "asset/audio/background.wav"

#define ICON_PATH "./asset/graphic/mac1024_2.png"
// "./asset/graphic/mac1024.png"

#define PLAYER_SPRITE_SHEET_PATH "./asset/graphic/idle.png"
#define ARROW_SEMICIRCLE_PATH "./asset/graphic/arroMini.png"
#define ARROW_STRAIGHT_PATH "./asset/graphic/rrowig.png"
#define CUBE_FIELD_PATH "./asset/graphic/AllTestTileWater.png"
#define CUBE_OBJECT_PATH "./asset/graphic/simples_test.png"

#define CUBE_SIDE_BACKGROUND_PATH(side) ("./asset/graphic/Cube" + std::to_string(side) + ".png").c_str()