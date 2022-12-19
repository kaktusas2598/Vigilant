#ifndef __Manipulator__
#define __Manipulator__

#include "Entity.hpp"
#include "Vector2D.hpp"

#include <vector>
#include <memory>

namespace Vigilant {

    // TODO
    // Testing out automation/ behaviours/ manipulators on entities
    // Beginning of 'AI'?
    // Lua co-routines
    // and so on...

    struct Manipulator {
        Entity* entity;
        virtual bool update(float elapsedTime) = 0;
        bool complete = false;
        bool active = false;
    };

    // Linearly interpolates position of Entity
    struct MoveManipulator : Manipulator {
        Vector2D startPos;
        Vector2D targetPos;
        float completionTime = 0.0f;
        float timePassed = 0.0f;

        MoveManipulator(Entity* e, Vector2D target, float fTime) {
            entity = e;
            startPos = e->transform->getPosition();
            targetPos = target;
            completionTime = fTime;
        }

        bool update(float elapsedTime) override {
            timePassed += elapsedTime;
            // Calculate new pos based on time using linear interpolation
            Vector2D newPos = targetPos - startPos;
            newPos *= (timePassed / completionTime);
            newPos += startPos;
            entity->transform->setPosition(newPos);

            if (timePassed >= completionTime) {
                entity->transform->setPosition(targetPos);
                complete = true;
            }
        }
    };


}

#endif // __Manipulator__
