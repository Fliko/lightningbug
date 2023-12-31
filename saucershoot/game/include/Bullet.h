#include "Object.h"
#include "EventCollision.h"

class Bullet : public df::Object {
    private:
        void out();
        void hit(const df::EventCollision *evt);

    public:
        Bullet(df::Vector hero_pos);
        int eventHandler(const df::Event *p_e) override;

};
