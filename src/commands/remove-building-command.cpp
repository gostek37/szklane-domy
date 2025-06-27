#include "remove-building-command.h"

bool RemoveBuildingCommand::execute() {
	if (auto& previous= game_->grid->getObjectsAt(gridPos_).front()->getComponent<TileComponent>()->building) {
		previousBuilding_ = previous->data;
		gridPos_ = previous->mainOwner->gridCoords;
	}
	return game_->constructionManager->destroy(gridPos_);
}

bool RemoveBuildingCommand::undo() {
	if (previousBuilding_) {
		return game_->constructionManager->build(previousBuilding_, gridPos_);
	}

	return false;
}
