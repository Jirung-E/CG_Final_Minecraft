#include "Minecraft/Scene/GameScene.h"
#include "Minecraft/GameManager.h"


int main(int argc, char** argv) {
    Log::print_log = false;
    Game::init(argc, argv);
    Game* gm = new Main { };
    gm->run();
}
