#include "RemoteLevelFetch.hpp"

#include <utility>
#include "../stringhelper.hpp"
#include "../lib/HTTPRequest.hpp"

RemoteLevelFetch *fetch = nullptr;

RemoteLevelFetch *RemoteLevelFetch::getInstance() {
    if (fetch == nullptr)
        fetch = new RemoteLevelFetch();
    return fetch;
}

Vector<std::string> RemoteLevelFetch::getLevelData() {
    return levels;
}

std::string RemoteLevelFetch::getHttpRequestBody(const std::string &url, int *status) {
    try {
        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request request{url};

        // send a get request
        const auto response = request.send("GET");
        if (status != nullptr) *status = response.status.code;
        return std::string{response.body.begin(), response.body.end()};
    }
    catch (const std::exception &e) {
        std::cerr << "Request failed, error: " << e.what() << '\n';
        return "";
    }
}

Vector<std::string> RemoteLevelFetch::getLevelUrls(int *status) {
    std::string body = getHttpRequestBody(REMOTE_LEVELS_LIST_URL, status);
    if (*status == 404) return {};
    return split(body, '\n');
}

std::string RemoteLevelFetch::fetchLevel(const std::string &url) {
    return getHttpRequestBody(url);
}

void RemoteLevelFetch::startLoading() {
    loader = std::thread(&RemoteLevelFetch::fetchLevels, this);
    loader.detach();
    state = LoadingState::LOADING;
}

RemoteLevelFetch::LoadingState RemoteLevelFetch::getLoadingState() {
    return state;
}

void RemoteLevelFetch::fetchLevels() {
    state = LoadingState::LOADING;
    int status = 0;
    auto levelUrls = getLevelUrls(&status);
    if (levelUrls.empty() && status != 200) {
        state = LoadingState::ERROR;
        return;
    }
    levels = {};
    for (const auto &url: levelUrls) {
        auto level = fetchLevel(REMOTE_LEVELS_BASE_URL + url);
        if (!level.empty())
            levels.push_back(level);
    }
    state = LoadingState::FINISHED;
}

Vector<std::string> RemoteLevelFetch::fetchLevelData() {
    fetchLevels();
    return getLevelData();
}

std::string RemoteLevelFetch::getLoadingStateString() {
    switch (state) {
        case LoadingState::NOT_STARTED:
            return "not loading";
        case LoadingState::LOADING:
            return "loading";
        case LoadingState::FINISHED:
            return "finished";
        case LoadingState::ERROR:
            return "error";
    }
}

RemoteLevelFetch::~RemoteLevelFetch() {

}

RemoteLevelFetch::RemoteLevelFetch() {
    levels = {};
}

void RemoteLevelFetch::waitForFinish() {
    if (loader.joinable())
        loader.join();
}
