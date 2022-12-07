#include "LevelLoader.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeMap.hpp"
#include <string>
#include <fstream>
#include "../GameObjects/CubeMapSide.hpp"

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

class CubeMapSide;

#define LINE_INDEX_NAME 0
#define LINE_INDEX_ID (LINE_INDEX_NAME + 1)
#define LINE_INDEX_CUBESIDES_START (LINE_INDEX_ID + 1)
#define LINE_INDEX_CUBESIDES_END (LINE_INDEX_CUBESIDES_START + 5)
#define LINE_INDEX_WORLDMAP (LINE_INDEX_CUBESIDES_END + 1)
#define LINE_INDEX_CUBEPOS (LINE_INDEX_WORLDMAP + 1)
#define LINE_INDEX_PLAYERPOS (LINE_INDEX_CUBEPOS + 1)

LevelLoader::LoadedLevelData LevelLoader::loadLevel(const std::string &path)
{

    Vector<CubeMapSide *> sides;
    Point worldSize = {0, 0};
    Vector<WorldField::WorldFieldEnum> worldField;
    Point cubePos = {0, 0};
    Point playerPos = {1, 1};
    int cubeSide = 2;
    std::string levelName;
    int id = 0;

    std::ifstream is(path);
    is >> levelName;
    is >> id;
    for (int s = 1; s < 7; s++)
    {
        int width, height, sideID;
        Vector<CubeField *> cubeFields;
        is >> width;
        is >> height;
        sideID = s;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int inType = -1;
                is >> inType;
                CubeField *temp = nullptr;
                Vector<CubeObject *> vec_temp;
                switch (inType)
                {
                case 0:
                    temp = new EmptyField(sideID, x, y);
                    break;
                case 1:
                    vec_temp.push_back(new Flag());
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 2:
                    temp = new Wall(sideID, x, y);
                    break;
                case 3:
                    temp = new Wall(sideID, x, y);
                    break;
                case 4:
                    temp = new PressurePlate(sideID, x, y, 1);
                    break;
                case 5:
                    vec_temp.push_back(new Slider(MovementDirection::moveToBigX, 1));
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 6:
                    vec_temp.push_back(new Slider(MovementDirection::moveToSmallX, 1));
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 7:
                    vec_temp.push_back(new Slider(MovementDirection::moveToBigY, 1));
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 8:
                    vec_temp.push_back(new Slider(MovementDirection::moveToSmallY, 1));
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 9:
                    vec_temp.push_back(new Magnet());
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 10:
                    vec_temp.push_back(new Stone());
                    temp = new EmptyField(sideID, x, y, vec_temp);
                    break;
                case 11:
                    temp = new ObjectBarrier(sideID, x, y);
                    break;
                default:
                    temp = new EmptyField(sideID, x, y);
                    break;
                }
                cubeFields.push_back(temp);
            }
        }
        auto *sideTemp = new CubeMapSide(cubeFields, width, height, sideID);
        sides.push_back(sideTemp);
    }
    is.close();
    return {.path = path, .name = levelName, .id = id, .sides = sides, .worldSize = worldSize, .worldField = worldField, .cubePos = cubePos, .playerPos = playerPos, .cubeSide = cubeSide};
}
