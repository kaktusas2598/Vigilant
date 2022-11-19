#ifndef __EventManager__
#define __EventManager__

#include "Event.hpp"
#include <list>
#include <functional>

namespace Vigilant {

    class EventManager {
        public:
            void addListener(EventId eventId, std::function<void(Event&)> const listener) {
                listeners[eventId].push_back(listener);
            }

            void sendEvent(Event& event) {
                uint32_t type = event.getType();
                for(auto const& listener: listeners[type]) {
                    listener(event);
                }
            }

            void sendEvent(EventId eventId) {
                Event event(eventId);

                for(auto const& listener: listeners[eventId]) {
                    listener(event);
                }
            }
        private:
            std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
    };
}

#endif