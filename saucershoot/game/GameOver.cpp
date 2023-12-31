#include "GameOver.h"
#include "GameStart.h"

#include "Event.h"
#include "GameManager.h"
#include "ObjectList.h"
#include "ObjectListIterator.h"
#include "ViewObject.h"
#include "EventStep.h"
#include "WorldManager.h"

GameOver::GameOver() {
   setType("GameOver");
   setSprite("gameover");

   if (setSprite("gameover") == 0)
       time_to_live = getAnimation().getSprite()->getFrameCount() *
           getAnimation().getSprite()->getSlowdown();
   else
       time_to_live = 0;

   registerInterest(df::STEP_EVENT);

   setLocation(df::CENTER_CENTER);
}

GameOver::~GameOver() {
    df::ObjectList obj_list = WM.getAllObjects(true);
    df::ObjectListIterator i(&obj_list);
    for (i.first(); !i.isDone(); i.next()) {
        df::Object *p_o = i.currentObject();
        if (p_o->getType() == "Saucer" || p_o->getType() == "ViewObject")
            WM.markForDelete(p_o);
        if (p_o->getType() == "GameStart") {
            p_o->setActive(true);
            dynamic_cast<GameStart *>(p_o) -> playMusic();
        }
    }
}

int GameOver::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    return 0;
}

void GameOver::step() {
    time_to_live--;
    if (time_to_live <= 0)
        WM.markForDelete(this);
}

int GameOver::draw() {
    return df::Object::draw();
}
