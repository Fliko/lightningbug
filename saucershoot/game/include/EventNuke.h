#include "Event.h"
#include <string>

const std::string NUKE_EVENT = "nuke";

class EventNuke : public df::Event {
    public:
        EventNuke();
};
