#include "Points.h"
#include "Color.h"
#include "ViewObject.h"
#include "EventStep.h"

Points::Points() {
    setLocation(df::TOP_RIGHT);
    setViewString(POINTS_STRING);
    setColor(df::YELLOW);

    registerInterest(df::STEP_EVENT);
}

int Points::eventHandler(const df::Event *p_e) {
    if (df::ViewObject::eventHandler(p_e))
        return 1;

    if (p_e->getType() == df::STEP_EVENT) {
        int step_count = dynamic_cast<const df::EventStep *>(p_e)->getStepCount();
        if (step_count%30 == 0)
            setValue(getValue()+1);
        return 1;
    }
    return 0;
}
