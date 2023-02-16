//
//  language.hpp
//  cube_puzzle iOS
//
//  Created by Tom Arlt on 15.02.23.
//
#pragma once
#include <string>
#include <vector>

enum class Language
{
    UNDEFINED,
    ENGLISH,
    GERMAN
};

Language getLanguage();
void setLanguage(Language language);

std::string getLanguageString(Language language);

std::vector<Language> getAvailableLanguages();

int getLanguageIndex(Language language);