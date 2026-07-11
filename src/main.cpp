//how to run
//cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
//cmake --build build --config Debug

#include "Game.h"
#include <iostream>
int main()
{
    Game game;
    if(!game.init()){
        std::cout << "Error initializing game" << std::endl;
        return 1;
    }
    game.run();
    game.clean();
    return 0;
}