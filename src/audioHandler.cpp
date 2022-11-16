#include "audioHandler.hpp"

AudioPlayer::AudioPlayer() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	background = Mix_LoadMUS("asset/audio/background.wav");


}

void AudioPlayer::playBackground(){
    Mix_PlayMusic(background, -1);
}

AudioPlayer::~AudioPlayer() {

	Mix_FreeMusic(background);

	//Quit SDL subsystems
	Mix_Quit();
}