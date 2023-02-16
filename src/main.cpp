#include "global.hpp"
#include "CubeGame.hpp"
#include "data/DiceData.hpp"
#include "config.hpp"
#include "data/RemoteLevelFetch.hpp"
#include "data/language.hpp"

void tests()
{
    /*auto data = DiceData();
    cout << data.toString() << std::endl;
    data.rotate(DiceRollDirection::NORTH);
    cout << data.toString() << std::endl;
    data.rotate(DiceRollDirection::WEST);
    cout << data.toString() << std::endl;*/
    // cout << RemoteLevelFetch::getInstance()->getHttpRequestBody(REMOTE_LEVELS_LIST_URL);
    // cout << RemoteLevelFetch::getInstance()->getLevelData()[0];
}

#if defined(__IPHONEOS__) || defined(__TVOS__)

int SDL_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
try
{
    tests();
    CubeGame game;
    RemoteLevelFetch::getInstance()->startLoading();
    return game.Run();
}
catch (...)
{
    std::cout << "Non-SDL Exception has occured!" << std::endl;

    // put a breakpoint in this line, to prevent the shell from closing
    return 66;
}
