#include "WorldField.hpp"

/// since the .txt file has a different char for each field type, this method converts them to their respective enum.
WorldField::WorldFieldEnum WorldField::convertCharToEnum(char c) {
    switch (c) {
        case 'd':
            return WorldField::WorldFieldEnum::DEFAULT;
        case 'l':
            return WorldField::WorldFieldEnum::LAVA;
        case 'i':
            return WorldField::WorldFieldEnum::ICE;
        //add here if a new WorldFieldEnum is added!
        default:
            std::cout << "Error in WorldField::WorldFieldEnum::convertCharToEnum" << std::endl;
            return WorldField::WorldFieldEnum::DEFAULT;
    }
}

Vector<char> WorldField::getListOfAllCharsLinkedToEnum() {
    Vector<char> list = Vector<char>();
    list.emplace_back('d');
    list.emplace_back('l');
    list.emplace_back('i');
    //add here if a new WorldFieldEnum is added!
    return list;
}

/// Make an enum printable. Intended for debugging purposes only!
char WorldField::convertEnumToChar(WorldField::WorldFieldEnum worldFieldEnum) {
    switch (worldFieldEnum) {
        case WorldFieldEnum::DEFAULT:
            return 'd';
        case WorldFieldEnum::LAVA:
            return 'l';
        case WorldFieldEnum::ICE:
            return 'i';
        //add here if a new WorldFieldEnum is added!
        default:
            std::cout << "Error in WorldField::WorldFieldEnum::convertEnumToChar" << std::endl;
            return 'd';
    }
}