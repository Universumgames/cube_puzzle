#include "AudioHandler.hpp"
#include "data/paths.hpp"

AudioPlayer::AudioPlayer() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    background = Mix_LoadMUS(MUSIC_BACKGROUND_PATH);


}

void AudioPlayer::playBackground() {
    Mix_PlayMusic(background, -1);
    playing = true;
}

AudioPlayer::~AudioPlayer() {

    Mix_FreeMusic(background);

    //Quit SDL subsystems
    Mix_Quit();
}

void AudioPlayer::pauseMusic() {
    Mix_PauseMusic();
    playing = false;
}
