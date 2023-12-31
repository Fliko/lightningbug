#include "Explosion.h"
#include "Event.h"
#include "EventStep.h"
#include "Object.h"
#include "WorldManager.h"

Explosion::Explosion() {
    if (setSprite("explosion") == 0)
        time_to_live = getAnimation().getSprite()->getFrameCount();
    else
        time_to_live = 0;

    setSolidness(df::SPECTRAL);
    registerInterest(df::STEP_EVENT);
}

void Explosion::step() {
    time_to_live--;
    if (time_to_live < 0)
        WM.markForDelete(this);
}

int Explosion::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    return 0;
}
