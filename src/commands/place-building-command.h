#pragma once
#include "command.h"
#include "game-object.h"
#include <memory>
#include <vector>
#include <SFML/Main.hpp>
#include "building-data.h"

/**
* \brief A command for placing a building.
*/
class PlaceBuildingCommand : public ICommand {
public:
	PlaceBuildingCommand(unsigned int buildingID, sf::Vector2i position, Game* game);
	PlaceBuildingCommand(std::shared_ptr<BuildingData> data, sf::Vector2i position, Game* game);

	bool execute() override;
	bool undo() override;

private:
	unsigned int buildingID_;
	Game* game_;
	sf::Vector2i gridPos_;
	std::shared_ptr<BuildingData> data_;
};