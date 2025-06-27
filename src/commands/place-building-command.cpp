#include "place-building-command.h"
#include "tile-component.h"

PlaceBuildingCommand::PlaceBuildingCommand(unsigned int buildingID, sf::Vector2i position, Game* game):
	buildingID_(buildingID), gridPos_(position), game_(game), data_(nullptr) {}

PlaceBuildingCommand::PlaceBuildingCommand(std::shared_ptr<BuildingData> data, sf::Vector2i position, Game* game): 
	buildingID_(0), gridPos_(position), game_(game), data_(data) {}

bool PlaceBuildingCommand::execute() {
	if (data_) {
		return game_->constructionManager->build(data_, gridPos_);
	}
	return game_->constructionManager->build(buildingID_, gridPos_);
}

bool PlaceBuildingCommand::undo() {
	return game_->constructionManager->destroy(gridPos_);
}