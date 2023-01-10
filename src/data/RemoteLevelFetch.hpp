#pragma once

#include "../global.hpp"
#include <thread>

#define REMOTE_LEVELS_BASE_URL "http://git.mt32.net/universum/cube_puzzle/-/raw/main"
#define REMOTE_LEVELS_LIST_URL REMOTE_LEVELS_BASE_URL "/remoteLevels/levelList.txt"

class RemoteLevelFetch {
private:
    RemoteLevelFetch();
    virtual ~RemoteLevelFetch();
public:
    static RemoteLevelFetch *getInstance();
public:
    Vector<std::string> getLevelData();
    Vector<std::string> fetchLevelData();
    std::string getHttpRequestBody(const std::string &url, int *status = nullptr);


    void startLoading();

    enum class LoadingState {
        NOT_STARTED,
        LOADING,
        FINISHED,
        ERROR
    };

    LoadingState getLoadingState();
    std::string getLoadingStateString();

private:
    std::thread loader;
    Vector<std::string> levels = {};
    LoadingState state = LoadingState::NOT_STARTED;
    std::string fetchLevel(const std::string &url);
    void fetchLevels();
    Vector<std::string> getLevelUrls();

};