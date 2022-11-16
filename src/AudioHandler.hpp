#pragma once

#include "global.hpp"

class AudioPlayer {
public:
	AudioPlayer();
	~AudioPlayer();
    void playBackground();
	//The music that will be played
	Mix_Music* background = NULL;
};
