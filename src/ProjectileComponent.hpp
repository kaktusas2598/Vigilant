#ifndef __ProjectileComponent__
#define __ProjectileComponent__

#include "Component.hpp"
#include "Vector2D.hpp"

namespace Vigilant {

    class ProjectileComponent : public Component {
        public:
            ProjectileComponent(Entity* owner);

            void load(Entity* e, int rng, int sp, Vector2D vel);

            void update(float deltaTime);
            void render() {}

            float getVelocityX() { return velocity.getX(); }
			float getVelocityY() { return velocity.getY(); }
			Vector2D getVelocity() { return velocity; }
        private:
            int range;
            int speed;
            int distance;
            Vector2D velocity;

            // Not to be confused with owner, owner is an actual projectile entity
            // shooter is the Entity which spawned this projectile
            Entity* shooter;

    };
}

#endif // __ProjectileComponent__
