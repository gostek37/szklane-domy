#include "game-object.h"

GameObject::GameObject(const sf::Vector2f& position, const sf::Vector2f& scale, std::shared_ptr<Grid> grid, Game* game): components_(), game(game) {
	setPosition(position);
	setScale(scale);
	// half-assed clamping for now TODO improve it
	gridCoords = grid->gridPositionClamped(grid->worldToGridInt(position));
}

GameObject::~GameObject() {
	// it is done this way so that the components decide themselves when to destroy (so that their managers don't break (looking at you, graphics component))
	for (auto& comp : components_) {
		send(Message(MessageType::COMPONENT_REMOVAL, comp));
	}
}

void GameObject::addComponent(std::shared_ptr<Component> component) {
	components_.push_back(component);
	components_.back()->setOwner(shared_from_this());
	component->initialize_();
	send(Message(MessageType::COMPONENT_ADDITION, component));
}

void GameObject::removeComponent(Component* component) {
	for (int i = 0; i < components_.size(); i++) {
		if (components_[i].get() != component) {
			continue;
		}

		std::cout << "Deleting component!\n";

		send(Message(MessageType::COMPONENT_REMOVAL, components_[i]));

		components_[i].reset();
		components_[i] = components_.back();
		components_.pop_back();
		break;
	}
}

void GameObject::send(const Message& message) {
	for (auto& component : components_) {
		component->receive_(message);
	}
}