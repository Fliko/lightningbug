#include "Star.h"

#include "Color.h"
#include "Event.h"
#include "EventOut.h"
#include "Object.h"
#include "Vector.h"
#include "WorldManager.h"
#include "DisplayManager.h"

Star::Star() {
    setType("Star");
    setSolidness(df::SPECTRAL);

    setVelocity(df::Vector((float) -1.0 /(rand()%10 +1), 0));
    setAltitude(0);
    df::Vector p((float) (rand()%(int)WM.getBoundary().getHorizontal()),
            (float) (rand()%(int)WM.getBoundary().getVertical()));
    setPosition(p);
}

int Star::draw() {
    return DM.drawCh(getPosition(), STAR_CHAR, df::WHITE);
}

void Star::out() {
    df::Vector p((float) (rand()%(int)WM.getBoundary().getHorizontal()),
            (float) (rand()%(int)WM.getBoundary().getVertical()));
    setPosition(p);
    setVelocity(df::Vector((float) -1.0 / (rand()%10 +1), 0));
}

int Star::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::OUT_EVENT) {
        out();
        return 1;
    }

    return 0;
}
