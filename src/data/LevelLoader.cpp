#include "LevelLoader.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeMap.hpp"
#include <string>
#include <fstream>
#include <iostream>

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
    int cubeSide = 0;
    std::string levelName;
    int id = 0;

    std::ifstream is(path);
    is >> levelName;
    is >> id;
    for (int s = 0; s < 6; s++)
    {
        CubeMapSide *side_temp = new CubeMapSide();

        is >> side_temp->width;
        is >> side_temp->height;
        side_temp->sideID = s;

        for (int x = 0; x < side_temp->width; x++)
        {
            for (int y = 0; y < side_temp->height; y++)
            {
                int inType = -1;
                is >> inType;
                // du darfst keine variablen in einem case erstellen.....
                // und momentan wird immer ein empty erstellt obwohl er nicht immer verwendet wird...
                // so, jetzt sollte das gehen ^^
                CubeField *temp = nullptr;
                switch (inType)
                {
                case 0:
                    side_temp->cubeFields.push_back(new EmptyField());
                    break;
                case 1:
                    temp = new EmptyField();
                    temp->cubeObjects.push_back(new Flag());
                    side_temp->cubeFields.push_back(temp);
                    break;
                case 2:
                    side_temp->cubeFields.push_back(new Wall_1());
                    break;
                case 3:
                    side_temp->cubeFields.push_back(new Wall_2());
                    break;
                case 4:
                    side_temp->cubeFields.push_back(new PressurePlate());
                    break;
                case 5:
                    temp = new EmptyField();
                    temp->cubeObjects.push_back(new Slider());
                    side_temp->cubeFields.push_back(temp);
                    break;
                case 6:
                    temp = new EmptyField();
                    temp->cubeObjects.push_back(new Magnet());
                    side_temp->cubeFields.push_back(temp);
                    break;
                case 7:
                    temp = new EmptyField();
                    temp->cubeObjects.push_back(new Stone());
                    side_temp->cubeFields.push_back(temp);
                    break;
                default:
                    side_temp->cubeFields.push_back(new EmptyField());
                    break;
                }
            }
        }
        sides.push_back(side_temp);
    }
    is.close();
    return {.path = path, .name = levelName, .id = id, .sides = sides, .worldSize = worldSize, .worldField = worldField, .cubePos = cubePos, .playerPos = playerPos, .cubeSide = cubeSide};
}
