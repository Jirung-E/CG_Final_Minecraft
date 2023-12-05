#include "Minecraft/Test.h"


int main(int argc, char** argv) {
    //Log::print_log = false;
    Game::init(argc, argv);
    Game* gm = new Test { };
    gm->run();
}
