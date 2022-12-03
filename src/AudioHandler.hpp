#pragma once

#include "global.hpp"

class AudioHandler;

class AudioPlayer {
public:
    explicit AudioPlayer(const char *filePath);
    void playOnce();
    void pause();
    void playLoop();
    bool isPlaying();

private:
    bool playing;

    bool wantsToPlay = false;

    bool looping = false;

    // sdl mixer data
    Mix_Chunk *chunk;
    int channel;

    void resume();
    void tempPause();

    friend class AudioHandler;
};

class AudioHandler {
private:
    AudioHandler();
    ~AudioHandler();
public:
    AudioHandler(const AudioHandler &) = delete;
public:
    static AudioHandler *getInstance();

    void init();
    void playAll();
    void pauseAll();

    void enableAudio(bool enabled);
    bool getAudioEnabled();

    Vector<AudioPlayer *> getPlayers();
private:
    Vector<AudioPlayer *> players;
    bool isInit = false;

    bool enabled = false;
private:
    friend class AudioPlayer;
};
