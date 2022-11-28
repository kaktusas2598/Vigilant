#ifndef __Component__
#define __Component__

namespace Vigilant {

    class Entity;

    /**
     * Different from traditional ECS model where components only hold data, here we will provide data and functionality 
     */
    class Component {
        public:
            Component(Entity* owner): owner(owner) {}

            virtual void update(float deltaTime) {};
            virtual void render() {};
        protected:
            Entity* owner;
    };
}

#endif // __Component__