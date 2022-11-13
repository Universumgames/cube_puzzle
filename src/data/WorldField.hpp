#pragma once
#include "../global.hpp"

class WorldField {
public:
    /// World Field enum
    enum class WorldFieldEnum : int {
        DEFAULT,
        LAVA,
        ICE
        //add new field types here
    };
    
    static WorldFieldEnum convertCharToEnum(char c);
    
    static Vector<char> getListOfAllCharsLinkedToEnum();
    
    static char convertEnumToChar(WorldFieldEnum worldFieldEnum);
    
};