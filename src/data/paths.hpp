#pragma once

// settings file containing all paths used in game

#define LEVELS_DIR "asset/levels/"
#define LEVELS_TRANSLATION_PATH(language) ("asset/levels/" + getLanguageString(language) + "/").c_str()
#define TITLESCREEN_LEVEL_PATH LEVELS_DIR "title.screen"
#define TITLESCREEN_LEVEL_SOLUTION_PATH LEVELS_DIR "titlescreen.solution"
#define LEVEL_SELECTOR_LOCALIZATION_PATH(language) ("asset/lang/selector/" + getLanguageString(language) + ".lang").c_str()

#define ROBOTO_FONT_FILEPATH "asset/font/RobotoSlab-Bold.ttf"
#define ROBOTO_FONT_LIGHT_FILEPATH "asset/font/Roboto/Roboto-Regular.ttf"

#define MUSIC_BACKGROUND_PATH "asset/audio/background.wav"
#define MUSIC_HIT_WALL_PATH "asset/audio/hit2.wav"
#define MUSIC_PRESSURE_PLATE_PRESSED "asset/audio/pressed.wav"
#define MUSIC_LEVEL_FINISHED "asset/audio/finish.wav"

#define ICON_PATH "asset/graphic/mac1024_2.png"

#define LANGUAGE_FILE_PATH "asset/lang/language.conf"

#define PLAYER_SPRITE_SHEET_PATH "asset/graphic/playerSheet.png"
#define ARROW_SEMICIRCLE_PATH "asset/graphic/arroMini.png"
#define ARROW_STRAIGHT_PATH "asset/graphic/rrowig.png"
#define CUBE_FIELD_PATH "asset/graphic/AllTestTyle.png"
#define CUBE_OBJECT_PATH "asset/graphic/AllTestTyle.png"
#define TITLESCREEEN_GAME_BANNER_PATH "asset/graphic/titlescreen.png"

#define TOUCH_ARROW_PATH "asset/graphic/arrow.png"
#define TOUCH_EXIT_PATH "asset/graphic/exit.png"
#define TOUCH_MUSIC_PATH "asset/graphic/music_touch.png"
#define TOUCH_TUTORIAL_PATH "asset/graphic/tut_touch.png"
#define TOUCH_SHIFT_PATH "asset/graphic/shift_touch.png"
#define TOUCH_ENTER_PATH "asset/graphic/enter_touch.png"

#define CUBE_SIDE_BACKGROUND_PATH(side) ("asset/graphic/Cube" + std::to_string(side) + ".png").c_str()

#define FLAG_PATH(lang) ("asset/graphic/flag_" + getLanguageString(lang) + ".png").c_str()