#include "Color.h"
#include "EventKeyboard.h"
#include "EventStep.h"
#include "EventView.h"
#include "GameManager.h"
#include "Particle.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "Vector.h"
#include "WorldManager.h"
#include "EventMouse.h"

#include "Hero.h"
#include "Bullet.h"
#include "EventNuke.h"
#include "GameOver.h"
#include "utility.h"

Hero::Hero(){
    setSprite("ship");
    registerInterest(df::KEYBOARD_EVENT);
    registerInterest(df::STEP_EVENT);
    registerInterest(df::MSE_EVENT);
    setType("Hero");
    df::Vector p(7, WM.getBoundary().getVertical()/2);
    setPosition(p);
    move_slowdown = 2;
    move_countdown = move_slowdown;
    fire_slowdown = 15;
    fire_countdown = fire_slowdown;
    nuke_count = 1;

    p_reticle = new Reticle;
    p_reticle->draw();
}

Hero::~Hero() {
    new GameOver;
    WM.markForDelete(p_reticle);

    df::addParticles(df::SPARKS, getPosition(), 2, df::BLUE);
    df::addParticles(df::SPARKS, getPosition(), 2, df::YELLOW);
    df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
    df::addParticles(df::SPARKS, getPosition(), 3, df::RED);


    df::Sound *p_sound = RM.getSound("game over");
    if (p_sound)
        p_sound->play();

}

int Hero::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        const df::EventKeyboard *p_keyboard_event = dynamic_cast <const df::EventKeyboard *> (p_e);
        kbd(p_keyboard_event);
        return 1;
    }

    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    if (p_e->getType() == df::MSE_EVENT) {
            const df::EventMouse *evt = dynamic_cast<const df::EventMouse *>(p_e);
            mouse(evt);
            return 1;
    }

    return 0;
}

void Hero::kbd(const df::EventKeyboard *p_keyboard_event) {

    switch(p_keyboard_event->getKey()) {
        case df::Keyboard::W:       // up
            if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
                move(-1);
            break;
        case df::Keyboard::S:       // down
            if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
                move(+1);
            break;
        case df::Keyboard::Q:       // quit
            if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED) 
                WM.markForDelete(this);
            break;
        case df::Keyboard::SPACE:
            if (p_keyboard_event->getKeyboardAction() ==df::KEY_PRESSED)
                nuke();
            break;
        default: // Key not handled.
            return;
    };

    return;
}

void Hero::mouse(const df::EventMouse *evt) {
    if(evt->getMouseAction() == df::CLICKED && evt->getMouseButton() == df::Mouse::LEFT)
        fire(evt->getMousePosition());
}

// Move up or down.
void Hero::move(int dy) {

    // See if time to move.
    if (move_countdown > 0)
        return;
    move_countdown = move_slowdown;

    // If stays on window, allow move.
    df::Vector new_pos(getPosition().getX(), getPosition().getY() + dy);
    if ((new_pos.getY() > 3) && 
            (new_pos.getY() < WM.getBoundary().getVertical()))
        WM.moveObject(this, new_pos);
}

void Hero::fire(df::Vector target) {
    if (fire_countdown > 0)
        return;
    fire_countdown = fire_slowdown;

    df::Vector v = target - getPosition();
    v.normalize();
    v.scale(1);
    Bullet *p = new Bullet(getPosition());
    p->setVelocity(v);

    df::Sound *p_sound = RM.getSound("fire");
    if (p_sound)
        p_sound->play();
}

void Hero::nuke() {
    if (nuke_count == 0)
        return;
    nuke_count--;

    EventNuke nuke;
    WM.onEvent(&nuke);

    df::EventView ev("Nukes", -1, true);
    WM.onEvent(&ev);


    df::Sound *p_sound = RM.getSound("nuke");
    if (p_sound)
        p_sound->play();

}

void Hero::step() {
    move_countdown--;
    if (move_countdown < 0) 
        move_countdown = 0;

    fire_countdown--;
    if (fire_countdown < 0)
        fire_countdown = 0;
}
