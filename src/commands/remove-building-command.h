#pragma once
#include "command.h"
#include "building-data.h"
#include "game-object.h"
#include <memory>
#include <vector>
#include <SFML/Main.hpp>

/**
* \brief A command for removing a building.
*/
class RemoveBuildingCommand: public ICommand {
public:
	RemoveBuildingCommand(sf::Vector2i position, Game* game) : previousBuilding_(nullptr), game_(game), gridPos_(position) {};

	bool execute() override;
	bool undo() override;

private:
	std::shared_ptr<BuildingData> previousBuilding_;
	Game* game_;
	sf::Vector2i gridPos_;
};