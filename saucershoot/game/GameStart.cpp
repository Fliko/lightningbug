#include "GameStart.h"
#include "Hero.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Saucer.h"
#include "Points.h"

#include "Event.h"
#include "EventKeyboard.h"
#include "GameManager.h"
#include "ViewObject.h"
#include "WorldManager.h"

#define NUM_SAUCER 16

GameStart::GameStart() {
    setType("GameStart");
    int ok = setSprite("gamestart");
    if (ok < 0)
        LM.writeLog("GAME Sprite Not Loaded");
    setLocation(df::CENTER_CENTER);

    registerInterest(df::KEYBOARD_EVENT);

    p_music = RM.getMusic("start music");
    playMusic();
    setActive(true);
}

int GameStart::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        df::EventKeyboard *p_keyboard_event = (df::EventKeyboard *) p_e;
        switch (p_keyboard_event->getKey()) {
            case df::Keyboard::P:
                start();
                p_music->pause();
                break;
            case df::Keyboard::Q:
                GM.setGameOver();
                break;
            default:
                break;
        }
        return 1;
    }

    return 0;
}

void GameStart::start() {
    new Hero;

    for (int i=0; i<NUM_SAUCER; i++)
        new Saucer;

    new Points;

    df::ViewObject *p_vo = new df::ViewObject;
    p_vo->setLocation(df::TOP_LEFT);
    p_vo->setViewString("Nukes");
    p_vo->setValue(1);
    p_vo->setColor(df::YELLOW);

    setActive(false);
}

void GameStart::playMusic() {
    p_music->play();
}

int GameStart::draw() {
    return df::Object::draw();
}
