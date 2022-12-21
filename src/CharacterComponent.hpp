#ifndef __CharacterComponent__
#define __CharacterComponent__

#include "Component.hpp"
#include "ScriptEngine.hpp"
#include <math.h>

namespace Vigilant {

    // TODO: load these from config or something?
    constexpr int XP_BASE = 100;
    constexpr double XP_SCALE = 1.1d;
    // XP formula, XP = (XP_BASE * (currentLevel - 1)) ^ XP_SCALE

    // Animations could be tied to different character states :0 ?
    enum class CharacterState {
        IDLE = 0,
        ATTACKING,
        DEFENDING
    };

    class CharacterComponent : public Component {
        public:
            CharacterComponent(Entity* owner): Component(owner) {}

            void load(int health, int level, int attack, int defense) {
                this->hp = health;
                this->level = level;
                // Calculate current XP
                this->xp = pow(XP_BASE * (level - 1), XP_SCALE);
                this->attack = attack;
                this->defense = defense;
            }

            double xpToNextLevel() {
                return pow(XP_BASE * level. XP_SCALE) - xp;
            }

            void damage(int dmg) {
                hp -= dmg;
                if (hp < 0) {
                    // TODO: instead of destroy maybe fire an event which does additional stuff
                    // and is responsible for destroying an entity
                    owner->destroy();
                }
            }

             void gainXP(int xp) {
                 // TODO: Fire gain xp event
                 this->xp += xp;
                 if (xpToNextLevel() <= 0) {
                     level++;
                     // Fire level up event
                 }
             }

             int getHP() const { return hp; }
             int getXP() const { return xp; }
             int getLevel() const { return level; }
        private:
            int hp;
            int xp;
            int level;
            CharacterState state = CharacterState::IDLE;

            // TODO: this is just the beginning, we will need ItemComponent for weapoins and items, InventoryComponent
            // Some kind of combat system, which will also need improved animation support
            int attack;
            int defense;
    };
}

#endif // __CharacterComponent__
