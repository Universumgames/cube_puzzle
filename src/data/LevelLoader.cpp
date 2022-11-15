#include "LevelLoader.hpp"
#include "../recthelper.hpp"
#include "../global.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeMap.hpp"
#include <string>
#include "../stringhelper.hpp"
#include "../filehelper.hpp"

/* File Format:
 * name#
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
 * width,height,enum array (chars) (column after column, splitted by ';')
 *
 * <WorldMap>:
 * width,height,enum array (chars) (column after column, splitted by ';')
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
#define STRING_TO_REMOVE "\n"

#define LINE_INDEX_NAME 0
#define LINE_INDEX_CUBESIDES_START 1
#define LINE_INDEX_CUBESIDES_END 6
#define LINE_INDEX_WORLDMAP 7
#define LINE_INDEX_CUBEPOS 8
#define LINE_INDEX_PLAYERPOS 9
#define LINE_INDEX_CUBESIDE 10

LevelLoader::LoadedLevelData LevelLoader::loadLevel(std::string path) {
    std::string fileContent = getFileContent(path);
    fileContent = removeAll(fileContent, STRING_TO_REMOVE);
    fileContent = removeAll(fileContent, "\n");
    Vector<CubeMapSide *> sides;
    Point worldSize = {};
    Vector<WorldField::WorldFieldEnum> worldField;
    Point cubePos = {};
    Point playerPos = {};
    int cubeSide = 0;
    std::string levelName;
    int id = std::stoi(std::filesystem::path(path).filename().string());

    int lineIndex = 0;

    std::istringstream stream(fileContent);
    for (std::string line; std::getline(stream, line, ROW_DELIMITTER);) {
        auto elements = split(line, ELEMENT_DELIMITTER);
        if (lineIndex == LINE_INDEX_NAME) {
            levelName.assign(line);
        } else if (lineIndex >= LINE_INDEX_CUBESIDES_START && lineIndex <= LINE_INDEX_CUBESIDES_END) { // CubeMapSides
            CubeMapSide *side = (CubeMapSide *) calloc(1, sizeof(CubeMapSide));
            side->sideID = lineIndex - LINE_INDEX_CUBESIDES_START + 1;
            side->width = std::stoi(elements[0]);
            side->height = std::stoi(elements[1]);
            auto fields = split(elements[2], ARRAY_DELIMITTER);
            for (auto field: fields) {
                side->side.push_back(CubeField::decode(field));
            }
            sides.push_back(side);
        } else if (lineIndex == LINE_INDEX_WORLDMAP) { // worldmap
            worldSize.x = std::stoi(elements[0]);
            worldSize.y = std::stoi(elements[1]);
            auto fields = split(elements[2], ARRAY_DELIMITTER);
            for (const auto &c: fields) {
                worldField.push_back(WorldField::convertCharToEnum(c.c_str()[0]));
            }
        } else if (lineIndex == LINE_INDEX_CUBEPOS) {
            cubePos.x = std::stoi(elements[0]);
            cubePos.y = std::stoi(elements[1]);
        } else if (lineIndex == LINE_INDEX_PLAYERPOS) {
            playerPos.x = std::stoi(elements[0]);
            playerPos.y = std::stoi(elements[1]);
        } else if (lineIndex == LINE_INDEX_CUBESIDE) {
            cubeSide = std::stoi(elements[0]);
        }
        lineIndex++;
    }

    return {.path = path, .name = levelName, .id = id, .sides = sides, .worldSize = worldSize, .worldField = worldField, .cubePos = cubePos, .playerPos = playerPos, .cubeSide = cubeSide};
}
