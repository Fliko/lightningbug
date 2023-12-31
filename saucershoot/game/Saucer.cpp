#include "Saucer.h"
#include "Explosion.h"
#include "EventNuke.h"
#include "Points.h"

#include "EventCollision.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "WorldManager.h"
#include "EventOut.h"
#include "EventView.h"
#include <cstdlib>

void Saucer::out() {
	if (getPosition().getX() >= 0)
		return;
	moveToStart();

    new Saucer;
}

void Saucer::moveToStart() {
	df::Vector temp_pos;

	float world_horiz = WM.getBoundary().getHorizontal();
	float world_vert = WM.getBoundary().getVertical();

	temp_pos.setX(world_horiz + rand() % (int) world_horiz + 3.0);
	temp_pos.setY(rand() % (int) (world_vert-4) + 4.0);

    df::ObjectList collision_list = WM.getCollisions(this, temp_pos);
    while (!collision_list.isEmpty()) {
        temp_pos.setX(temp_pos.getX()+1);
        collision_list = WM.getCollisions(this, temp_pos);
    }

	WM.moveObject(this, temp_pos);
}

int Saucer::eventHandler(const df::Event *p_e) {

    if (p_e->getType() == df::COLLISION_EVENT) {
        const df::EventCollision *p_collision_event = dynamic_cast<const df::EventCollision *>(p_e);
        hit(p_collision_event);
        return 1;
    }

	if (p_e->getType() == df::OUT_EVENT) {
        out();
		return 1;
	}

    if (p_e->getType() == NUKE_EVENT) {
        LM.writeLog("NUKE EVENT SENT");
        WM.markForDelete(this);
        Explosion *p_exp = new Explosion;
        p_exp->setPosition(this->getPosition());
        if (rand()%3 == 0)
            new Saucer;
        return 1;
    }

	return 0;
}

void Saucer::hit(const df::EventCollision *p_c) {
    if (p_c->getObject1()->getType() == "Saucer" && p_c->getObject2()->getType() == "Saucer") {
        return;
    }

    if (p_c->getObject1()->getType() == "Hero" || p_c->getObject2()->getType() == "Hero") {
        WM.markForDelete(p_c->getObject1());
        WM.markForDelete(p_c->getObject2());
    }
    
    if (p_c->getObject1()->getType() == "Saucer" || p_c->getObject2()->getType() == "Saucer") {
        Explosion *p_exp = new Explosion;
        p_exp->setPosition(this->getPosition());

        df::Sound *p_sound = RM.getSound("explode");
        if (p_sound)
            p_sound->play();

        new Saucer;
    }
}

Saucer::Saucer() {
	// Setup "saucer" sprite
	setSprite("saucer");
    setType("Saucer");
	setVelocity(df::Vector(-0.25,0));

	// set starting location (middle)
	// int world_horiz = (int) WM.getBoundary().getHorizontal();
	// int world_vert = (int) WM.getBoundary().getVertical();
	// df::Vector p(world_horiz/2, world_vert/2);
	// setPosition(p);
	moveToStart();
    registerInterest(NUKE_EVENT);
}

Saucer::~Saucer() {
    df::EventView ev(POINTS_STRING, 10, true);
    WM.onEvent(&ev);
}
