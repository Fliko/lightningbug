#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Pause.h"

#include "Saucer.h"
#include "Hero.h"
#include "Points.h"
#include "Star.h"
#include "GameStart.h"

#define NUM_STAR 16

void loadResources(void) {
    RM.loadSprite("sprites/saucer-spr.txt", "saucer");
    RM.loadSprite("sprites/ship-spr.txt", "ship");
    RM.loadSprite("sprites/bullet-spr.txt", "bullet");
    RM.loadSprite("sprites/explosion-spr.txt", "explosion");
    RM.loadSprite("sprites/gameover-spr.txt", "gameover");
    RM.loadSprite("sprites/gamestart-spr.txt", "gamestart");

    RM.loadSound("sounds/fire.wav", "fire");
    RM.loadSound("sounds/explode.wav", "explode");
    RM.loadSound("sounds/nuke.wav", "nuke");
    RM.loadSound("sounds/game-over.wav", "game over");

    RM.loadMusic("sounds/start-music.wav", "start music");
}

void populateWorld(void) { 
   for (int i = 0; i < NUM_STAR; ++i)
       new Star;

   new GameStart();
}

int main(int argc, char *argv[]) {
    // start game manager
    if (GM.startUp()) {
        LM.writeLog("Error starting game manager!");
        GM.shutDown();
        return 1;
    }
    
    LM.setFlush(true);
    df::splash();

    // load resources
    loadResources();

    // populate game world
    populateWorld();

    // enable pausing
    new df::Pause(df::Keyboard::TAB);

    GM.run();
    GM.shutDown();

    return 0;
}
