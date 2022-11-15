#include "LevelLoader.hpp"
#include "../recthelper.hpp"
#include "../global.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeMap.hpp"
#include <string>
#include "../stringhelper.hpp"
#include "../filehelper.hpp"

/* File Format:
 * <CubeMapSide1>#
 * ...
 * <CubeMapSide6>#
 * <WorldMap>#
 * <CubePos>#
 * <PlayerPos>#
 * CubeSide
 *
 *
 * Definitions:
 * <CubeMapSideX>:
 * width,height,enum array (chars) (column after column, splitted by ' ')
 *
 * <WorldMap>:
 * width,height,enum array (chars) (column after column, splitted by ' ')
 *
 * <CubePos>:
 * x,y
 *
 * <PlayerPos>:
 * x,y
 *
 * <CubeSide>:
 * x
 *
 */

#define ROW_DELIMITTER '#'
#define ELEMENT_DELIMITTER ','
#define ARRAY_DELIMITTER ';'
#define STRING_TO_REMOVE " "

LevelLoader::LoadedLevelData LevelLoader::loadLevel(std::string path) {
    std::string fileContent = getFileContent(path);
    fileContent = removeAll(fileContent, STRING_TO_REMOVE);
    fileContent = removeAll(fileContent, "\n");
    Vector<CubeMapSide> sides;
    int worldW = 0, worldH = 0;
    Vector<WorldField::WorldFieldEnum> worldField;
    Point cubePos = {};
    Point playerPos = {};
    int cubeSide = 0;

    int lineIndex = 0;

    std::istringstream stream(fileContent);
    for (std::string line; std::getline(stream, line, ROW_DELIMITTER);) {
        auto elements = split(line, ELEMENT_DELIMITTER);
        if (lineIndex < 6) { // CubeMapSides
            CubeMapSide side;
            side.width = std::stoi(elements[0]);
            side.height = std::stoi(elements[1]);
            auto fields = split(elements[2], ARRAY_DELIMITTER);
            for(auto field: fields){
                side.side.push_back(CubeField::decode(field));
            }
            sides.push_back(side);
        } else if (lineIndex == 6) { // worldmap
            worldW = std::stoi(elements[0]);
            worldH = std::stoi(elements[1]);
            auto fields = split(elements[2], ARRAY_DELIMITTER);
            for(const auto& c: fields) {
                worldField.push_back(WorldField::convertCharToEnum(c.c_str()[0]));
            }
        } else if (lineIndex == 7) {
            cubePos.x = std::stoi(elements[0]);
            cubePos.y = std::stoi(elements[1]);
        } else if (lineIndex == 8) {
            playerPos.x = std::stoi(elements[0]);
            playerPos.y = std::stoi(elements[1]);
        } else if (lineIndex == 9) {
            cubeSide = std::stoi(elements[0]);
        }
        lineIndex++;
    }

    return {sides, worldW, worldH, worldField, cubePos, playerPos, cubeSide};
}
