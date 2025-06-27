#include "movement-component.h"
#include "game-object.h"
#include <SFML/Main.hpp>

void MovementComponent::receive_(const Message& message) {
	switch (message.type) {
		case (MessageType::MOVE_GRID): {
			// changes weak ptr into shared ptr so that the owner doesn't get deleted when removed from the grid
			auto owner = owner_.lock();
			game_->grid->removeObject(owner);
			sf::Vector2i newPos = std::get<sf::Vector2i>(message.argument);
			// for now I lack a grid to world function, so this will have to suffice xd
			// TODO add grid to world function
			owner->setPosition(game_->grid->getObjectsAt(newPos).front()->getPosition());
			game_->grid->addObject(owner);
			break;
		}
		default:
			break;
	}
}
