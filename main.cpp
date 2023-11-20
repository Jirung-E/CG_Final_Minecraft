#include "Minecraft/Test.h"


int main(int argc, char** argv) {
    Game::init(argc, argv);
    Game* gm = new Test { };
    gm->run();
}
