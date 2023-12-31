#include "EventCollision.h"
#include "Object.h"

class Saucer : public df::Object {

	public:
		Saucer();
        ~Saucer();
        void hit(const df::EventCollision *p_c);

	
	int eventHandler(const df::Event *p_e) override;
	void out();
	void moveToStart();
};

