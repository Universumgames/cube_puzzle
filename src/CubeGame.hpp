#pragma once

#include "gamebase.hpp"
#include "data/SpriteStorage.hpp"
#include "AudioHandler.hpp"

class LevelSelector;
class TitleScreen;

enum class ExitState {
    UNSET, FINISHED, CANCELLED
};
struct GameStateData {
    int sourceStateID = -1;
    ExitState exitState = ExitState::UNSET;
    char optionalDataTypeIdentifier = '\0';
    void *optionalData = nullptr;
    size_t optionalDataSize = 1;
};

/// Cube Game handler
class CubeGame final : public Game {
public:

public:
    CubeGame();

    AudioHandler* audioHandler;

    bool HandleEvent(const Event event) override;

    SpriteStorage *getSpriteStorage() override;

    /// should display debug info
    bool isDebug() const;

    Point getCurrentRenderTargetSize();

    /// return to level selector, set interGameStateData to send some data along
    void returnToLevelSelector();

    /// set gamestate data for next gamestate to load
    GameStateData interGameStateData;

    void setWindowName(std::string windowName);

private:
    SpriteStorage spriteStorage;

    /// load sprites into sprite storage
    void loadSprites();

    void setWindowIcon();

    /// this should be set to false in final version
    bool debugView = false;

    AudioPlayer* backgroundMusic;
protected:
    friend class LevelSelector;
    friend class TitleScreen;
};
