#include "component.h"

void Component::setOwner(std::shared_ptr<GameObject> owner) {
	owner_ = owner;
}