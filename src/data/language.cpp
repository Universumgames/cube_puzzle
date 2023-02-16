#include "language.hpp"
#include "./paths.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>

// read language from file in cpp way
Language getLanguage() {
    std::ifstream langFile(LANGUAGE_FILE_PATH);
    std::string langString;
    langFile >> langString;
    langFile.close();

    if (langString == "en") {
        return Language::ENGLISH;
    } else if (langString == "de") {
        return Language::GERMAN;
    } else {
        return Language::UNDEFINED;
    }
}

// write language to file
void setLanguage(Language language) {
    // save language to file
    std::ofstream langFile(LANGUAGE_FILE_PATH);
    langFile << getLanguageString(language);
    langFile.close();
}

std::string getLanguageString(Language language) {
    switch (language) {
        case Language::UNDEFINED:
            return "";
        case Language::ENGLISH:
            return "en";
        case Language::GERMAN:
            return "de";
    }
    return "";
}

std::vector<Language> getAvailableLanguages() {
    return {Language::ENGLISH, Language::GERMAN};
}

int getLanguageIndex(Language language) {
    auto languages = getAvailableLanguages();
    for (int i = 0; i < languages.size(); i++) {
        if (languages[i] == language) {
            return i;
        }
    }
    return -1;
}
