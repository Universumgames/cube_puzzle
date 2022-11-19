#include "LevelLoader.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeMap.hpp"
#include <string>
#include "../filehelper.hpp"

/* File Format:
 * name
 * id
 * <CubeMapSide1>
 * ...
 * <CubeMapSide6>
 * <WorldMap>
 * <CubePosOnWorldMap>
 * <PlayerPos>
 *
 *
 * Definitions:
 * <CubeMapSideX>:
 * width,height,enum array (chars) (column after column, top to bottom)
 *
 * <WorldMap>:
 * width,height,enum array (chars) (column after column, top to bottom)
 *
 * <CubePosOnWorldMap>:
 * x,y
 *
 * <PlayerPos>:
 * x,y, cubeside
 *
 */

#define LINE_INDEX_NAME             0
#define LINE_INDEX_ID               (LINE_INDEX_NAME + 1)
#define LINE_INDEX_CUBESIDES_START  (LINE_INDEX_ID + 1)
#define LINE_INDEX_CUBESIDES_END    (LINE_INDEX_CUBESIDES_START + 5)
#define LINE_INDEX_WORLDMAP         (LINE_INDEX_CUBESIDES_END + 1)
#define LINE_INDEX_CUBEPOS          (LINE_INDEX_WORLDMAP + 1)
#define LINE_INDEX_PLAYERPOS        (LINE_INDEX_CUBEPOS + 1)

LevelLoader::LoadedLevelData LevelLoader::loadLevel(const std::string& path) {
    std::string fileContent = getFileContent(path);
    Vector<CubeMapSide *> sides;
    Point worldSize = {};
    Vector<WorldField::WorldFieldEnum> worldField;
    Point cubePos = {};
    Point playerPos = {};
    int cubeSide = 0;
    std::string levelName;
    int id = 0;

    int objectIndex = 0;

    std::istringstream stream(fileContent);
    while (objectIndex <= LINE_INDEX_PLAYERPOS) {
        if (objectIndex == LINE_INDEX_NAME) {
            stream >> levelName;
        } else if (objectIndex == LINE_INDEX_ID) {
            stream >> id;
        } else if (objectIndex >= LINE_INDEX_CUBESIDES_START && objectIndex <= LINE_INDEX_CUBESIDES_END) { // CubeMapSides
            auto *side = new CubeMapSide();
            side->sideID = objectIndex - LINE_INDEX_CUBESIDES_START + 1;
            stream >> side->width;
            stream >> side->height;
            for (int x = 0; x < side->width; x++) {
                for(int y = 0; y < side->height; y++) {
                    std::string field;
                    stream >> field;
                    side->side.push_back(CubeField::decode(field));
                }
            }
            sides.push_back(side);
        } else if (objectIndex == LINE_INDEX_WORLDMAP) { // worldmap
            stream >> worldSize.x;
            stream >> worldSize.y;
            for (int x = 0; x < worldSize.x; x++) {
                for (int y = 0; y < worldSize.y; y++) {
                    char c;
                    stream >> c;
                    worldField.push_back(WorldField::convertCharToEnum(c));
                }
            }
        } else if (objectIndex == LINE_INDEX_CUBEPOS) {
            stream >> cubePos.x;
            stream >> cubePos.y;
        } else if (objectIndex == LINE_INDEX_PLAYERPOS) {
            stream >> playerPos.x;
            stream >> playerPos.y;
            stream >> cubeSide;
        }
        objectIndex++;
    }

    return {.path = path, .name = levelName, .id = id, .sides = sides, .worldSize = worldSize, .worldField = worldField, .cubePos = cubePos, .playerPos = playerPos, .cubeSide = cubeSide};
}
