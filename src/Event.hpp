#ifndef __Event__
#define __Event__

#include <stdint.h>
#include <unordered_map>
#include <any>

namespace Vigilant {


    using EventId = std::uint32_t;
    using ParamId = std::uint32_t;


    /**
     * Event List
    */
    const EventId INPUT = "InputEvent";
    const EventId QUIT = "QuitEvent";

    class Event {
        public:
            Event() = delete;

            explicit Event(EventId eventType)
                : type(eventType)
            {}

            template<typename T>
            void setParam(EventId id, T value) {
                data[id] = value;
            }

            template<typename T>
            T getParam(EventId id) {
                return std::any_cast<T>(data[id]);
            }

            EventId getType() {
                return type;
            }
        private:
            EventId type;
            std::unordered_map<EventId, std::any> data{};
    };
}

#endif