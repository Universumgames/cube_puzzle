#include "global.hpp"
#include "CubeGame.hpp"
#include "examplegame/examplegame.hpp"
#include "data/CubeData.hpp"

void tests(){
    auto data = CubeData();
    cout << data.toString() << std::endl;
    data.rotate(CubeRollDirection::NORTH);
    cout << data.toString() << std::endl;
    data.rotate(CubeRollDirection::WEST);
    cout << data.toString() << std::endl;
}

extern "C" int main(int argc, char *argv[])
try {
    tests();
    CubeGame game;
    return game.Run();
}
catch (...) {
    std::cout << "Non-SDL Exception has occured!" << std::endl;

    // put a breakpoint in this line, to prevent the shell from closing
    return 66;
}
