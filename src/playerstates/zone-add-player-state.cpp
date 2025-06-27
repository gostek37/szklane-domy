#include "zone-add-player-state.h"
#include "game.h"
#include <algorithm>
#include "glm/common.hpp"
#include "game-object.h"

void ZoneAddPlayerState::mousePress(sf::Vector2i pos) {
	mousePressed_ = true;
	initialPressPos_ = pos;
}

void ZoneAddPlayerState::mouseRelease() {
	mousePressed_ = false;
	if (getPrice() <= game_->cityData.money) {
		game_->tileComponentManager->addZone(currentSelection_, 0);
		game_->cityData.money -= getPrice();
	}

	clearSelection();
}

void ZoneAddPlayerState::update(float deltaTime) {
	lastMouseGridPos_ = currentMouseGridPos_;
	currentMouseGridPos_ = game_->grid->getMouseClamped();

	if (!mousePressed_) {
		return;
	}

	// TODO figure out why is the one tile always blank when uncommenting, but still getting added to the zone
	// if (lastMouseGridPos_ != currentMouseGridPos_) {
		changeSelection();
	// }
}

void ZoneAddPlayerState::optOut() {
	mousePressed_ = false;
	clearSelection();
}

void ZoneAddPlayerState::clearSelection() {
	for (std::shared_ptr<GameObject>& obj : currentSelection_) {
		obj->send(Message(MessageType::RESET_COLOUR));
	}
}

void ZoneAddPlayerState::changeSelection() {
	clearSelection();

	sf::Vector2i squareStart, squareSize;
	squareStart.x = std::max(std::min(currentMouseGridPos_.x, initialPressPos_.x), 0);
	squareStart.y = std::max(std::min(currentMouseGridPos_.y, initialPressPos_.y), 0);
	squareSize.x = abs(std::max(currentMouseGridPos_.x, 0) - initialPressPos_.x) + 1;
	squareSize.y = abs(std::max(currentMouseGridPos_.y, 0) - initialPressPos_.y) + 1;
	
	currentSelection_ = game_->grid->getTilesInSquare(squareStart, squareSize);

	glm::vec3 newColour;
	if (getPrice() <= game_->cityData.money) {
		newColour = glm::vec3(0.8, 0.85, 0.8);
	} else {
		newColour = glm::vec3(0.9, 0.5, 0.5);
	}

	for (std::shared_ptr<GameObject>& obj : currentSelection_) {
		obj->send(Message(MessageType::SET_COLOUR, newColour ));
	}
}

unsigned int ZoneAddPlayerState::getPrice() {
	return game_->tileComponentManager->zones[0]->price * currentSelection_.size();
}
