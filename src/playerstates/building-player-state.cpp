#include "building-player-state.h"
#include "game.h"
#include "place-building-command.h"
#include "movement-component.h"
#include <algorithm>
#include "building-data.h"

void BuildingPlayerState::mousePress(sf::Vector2i pos) {
	game_->commandHistory.runCommand(new PlaceBuildingCommand(currentBuilding_, pos, game_));
}

void BuildingPlayerState::mouseRelease() {};

void BuildingPlayerState::update(float deltaTime) {
	previewObject_->send(Message(MessageType::MOVE_GRID, game_->grid->getMouseClamped()));
}

void BuildingPlayerState::setOption(unsigned int option) {
	if (option - 1 >= game_->constructionManager->getBuildingData().size()) {
		return;
	}
	currentBuilding_ = game_->constructionManager->getBuildingData().at(option - 1);
	optOut();
	optIn();
}

void BuildingPlayerState::optIn() {
	previewObject_ = std::make_shared<GameObject>(GameObject(game_->mouseWorldPos, {1, 1}, game_->grid, game_));
	game_->grid->addObject(previewObject_);
	previewObject_->addComponent(std::make_shared<GraphicsComponent>(game_, currentBuilding_->textureName));
	previewObject_->addComponent(std::make_shared<MovementComponent>(game_));
}

void BuildingPlayerState::optOut() {
	game_->grid->removeObject(previewObject_);
	previewObject_.reset();
}
