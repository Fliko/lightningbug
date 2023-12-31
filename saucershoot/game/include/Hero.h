#include "EventMouse.h"
#include "Object.h"
#include "EventKeyboard.h"
#include "Vector.h"
#include "Reticle.h"

class Hero : public df::Object {

    private:
        int move_slowdown;
        int move_countdown;
        int fire_slowdown;
        int fire_countdown;
        int nuke_count;
        Reticle *p_reticle;

        void kbd(const df::EventKeyboard *p_keyboard_event);
        void mouse(const df::EventMouse *evt);
        void move(int dy);
        void step();
        void fire(df::Vector target);
        void nuke();

    public:
        Hero();
        ~Hero();
        int eventHandler(const df::Event *p_e) override;
};
