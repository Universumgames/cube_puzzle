#include "AudioHandler.hpp"
#include "data/paths.hpp"

AudioHandler::AudioHandler()
{
}

AudioHandler::~AudioHandler()
{
    // Quit SDL subsystems
    Mix_Quit();
}

AudioHandler *audioHandler = nullptr;

AudioHandler *AudioHandler::getInstance()
{
    if (audioHandler == nullptr)
        audioHandler = new AudioHandler();
    return audioHandler;
}

void AudioHandler::init()
{
    if (isInit)
        return;
    isInit = true;
    SDL_Init(SDL_INIT_AUDIO);
    // Initialize SDL_mixer
    if (int err = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s", err);
}

void AudioHandler::playAll()
{
    for (auto *player : players)
    {
        if (player != nullptr)
            player->playOnce();
    }
}

void AudioHandler::pauseAll()
{
    for (auto *player : players)
    {
        if (player != nullptr)
            player->pause();
    }
}

Vector<AudioPlayer *> AudioHandler::getPlayers()
{
    return players;
}

void AudioHandler::enableAudio(bool enabled) {
    this->enabled = enabled;
    if(enabled){
        for(auto player : players){
            player->resume();
        }
        cout << "play";
    }else{
        for(auto player : players){
            player->tempPause();
        }
        cout << "pause";
    }
}

bool AudioHandler::getAudioEnabled() {
    return enabled;
}

AudioPlayer::AudioPlayer(const char *filePath)
{
    chunk = Mix_LoadWAV(filePath);
    //music = Mix_LoadMUS(filePath);
    channel = -1;
    playing = false;
    AudioHandler::getInstance()->players.push_back(this);
}

void AudioPlayer::playOnce()
{
    looping = false;
    if(!audioHandler->getAudioEnabled()) return;
    channel = Mix_PlayChannel(-1, chunk, 0);
    //Mix_PlayMusic(music, 1);
    playing = true;
}

void AudioPlayer::pause()
{
    Mix_Pause(channel);
    playing = false;
    wantsToPlay = false;
}

void AudioPlayer::playLoop()
{
    wantsToPlay = true;
    looping = true;
    if(!audioHandler->getAudioEnabled()) return;
    channel = Mix_PlayChannel(-1, chunk, -1);
    playing = true;
}

bool AudioPlayer::isPlaying()
{
    return playing;
}

void AudioPlayer::resume() {
    if(!wantsToPlay) return;
    if(looping) playLoop();
    else playOnce();
}

void AudioPlayer::tempPause() {
    Mix_Pause(channel);
}
