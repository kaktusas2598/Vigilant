#ifndef __CharacterComponent__
#define __CharacterComponent__

#include "Component.hpp"
#include "LuaListener.hpp"
#include <math.h>

namespace Vigilant {

    // TODO: load these from config or something?
    constexpr int XP_BASE = 100;
    constexpr double XP_SCALE = 1.1d;

    // Animations could be tied to different character states :0 ?
    enum class CharacterState {
        IDLE = 0,
        ATTACKING,
        DEFENDING
    };

    // FIXME: Inherting from LuaListener is not such a good idea anymore
    // For this component we might want to have multiple events like onDamage(), onLevelUp(), etc
    class CharacterComponent : public Component, public LuaListener {
        public:
            CharacterComponent(Entity* owner): Component(owner), LuaListener() {}

            void load(int health, int level, int attack, int defense) {
                this->hp = health;
                this->level = level;
                // Calculate current XP
                this->xp = pow(XP_BASE * (level - 1), XP_SCALE);
                this->attack = attack;
                this->defense = defense;
            }

            void damage(int dmg) {
                hp -= dmg;
                if (hp < 0) {
                    owner->destroy();
                }
            }

             void gainXP(int xp) {
                 this->xp += xp;
                 // TODO: check for lvl up
             }

             int getHP() const { return hp; }
             int getXP() const { return xp; }
             int getLevel() const { return level; }
        private:
            int hp;
            int xp;
            int level;
            CharacterState state = CharacterState::IDLE;

            // TODO: relationship between level and XP
            // Example: xpToLevel = (xpBase * currentLevel) ^ scale - xp needed for next level

            // TODO: this is just the beginning, we will need ItemComponent for weapoins and items, InventoryComponent
            // Some kind of combat system, which will also need improved animation support
            int attack;
            int defense;
    };
}

#endif // __CharacterComponent__
