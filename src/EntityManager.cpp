#include "EntityManager.hpp"

namespace Vigilant {
    EntityManager* EntityManager::s_pInstance = 0;

    int EntityManager::livingCount = 0;
}