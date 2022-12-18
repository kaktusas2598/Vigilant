#include "IdComponent.hpp"

namespace Vigilant {

    int IdComponent::count = 0;

    IdComponent::IdComponent(Entity* owner): Component(owner), id(count++) {}

    IdComponent::~IdComponent() {}
}
