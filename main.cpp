#define NOMINMAX
#pragma comment(lib,"user32.lib")
#include <iostream>
#include <stdexcept>
#include <windows.h>
#include "Game.h"

using std::endl;
using std::cerr;
using std::runtime_error;
using rtd::Game;

int main(int argc, char* args[]) {
    try {
        Game game;
        game.run();
    }
    catch (runtime_error& e) {
        cerr << e.what() << endl;
        MessageBoxA(NULL, e.what(), "Error", 0x00000010L);
        return -1;
    }

    return 0;
}