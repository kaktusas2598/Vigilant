#ifndef __Component__
#define __Component__

#include <memory>
#include <bitset>
#include <array>

namespace Vigilant {

    class Entity;
    class Component;

    using ComponentID = std::size_t;

    inline ComponentID getComponentTypeID() {
        static ComponentID lastID = 0;
        return lastID++;
    }

    // Makes sure each component has a unique id associated to it, so it should be:
    // Id - 0, Transform = 1 and so on
    template <typename T> inline ComponentID getComponentTypeID() noexcept {
        static ComponentID typeID = getComponentTypeID();
        return typeID;
    }

    // 2022-12-16: current component count - 9
    constexpr std::size_t MAX_COMPONENTS = 32;

    // Will allow to get component without looping dynamic arrays and dynamic_cast
    using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
    using ComponentArray = std::array<Component*, MAX_COMPONENTS>;


    /**
     * Different from traditional ECS model where components only hold data, here we will provide data and functionality
     */
    class Component {
        public:
            // TODO: move load params to constructor? To utilise TArgs thingy
            Component(Entity* owner): owner(owner) {}
            virtual ~Component(){}

            virtual void init() {}
            virtual void preUpdate(float deltaTime) {}
            virtual void update(float deltaTime) {}
            virtual void render() {}

        protected:
            Entity* owner;
    };
}

#endif // __Component__
