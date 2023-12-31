#include "Bullet.h"
#include "EventCollision.h"
#include "EventOut.h"
#include "Object.h"
#include "WorldManager.h"

Bullet::Bullet(df::Vector hero_pos) {
    setSprite("bullet");
    setType("Bullet");

    df::Vector p(hero_pos.getX()+3, hero_pos.getY());
    setPosition(p);
    setSpeed(1);
    
    setSolidness(df::SOFT);
}

int Bullet::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::COLLISION_EVENT) {
        const df::EventCollision *p_collision_event = dynamic_cast<const df::EventCollision *>(p_e);
        hit(p_collision_event);
        return 1;
    }

    if (p_e->getType() == df::OUT_EVENT) {
        out();
        return 1;
    }

    return 0;
}

void Bullet::out() {
    WM.markForDelete(this);
}

void Bullet::hit(const df::EventCollision *evt) {
    if ((evt->getObject1()->getType() == "Saucer") || (evt->getObject2()->getType() == "Saucer")) {
        WM.markForDelete(evt->getObject1());
        WM.markForDelete(evt->getObject2());
    }
}
