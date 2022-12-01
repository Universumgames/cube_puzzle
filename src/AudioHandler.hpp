#pragma once

#include "global.hpp"

class AudioPlayer {
public:
    explicit AudioPlayer(const char *filePath);
    void playOnce();
    void pause();
    void playLoop();
    bool isPlaying();

private:
    bool playing;

    // sdl mixer data
    Mix_Chunk *chunk;
    int channel;
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

    Vector<AudioPlayer *> getPlayers();
private:
    Vector<AudioPlayer *> players;
    bool isInit = false;
private:
    friend class AudioPlayer;
};
