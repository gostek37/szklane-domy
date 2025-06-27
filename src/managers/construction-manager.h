#pragma once
#include "command-history.h"
#include "place-building-command.h"
#include <SFML/Main.hpp>
#include "building-data.h"
#include "data-parser.h"
#include <vector>

class GameObject;
class Game;

/**
* \brief Class managing everything building related
*
* \details This class ,after checking if the space is free, can build buildings, remove them and retrieve their data
*/
class ConstructionManager {
public:
	ConstructionManager(Game* game) : game_(game), buildingData_(DataParser::getBuildingData()) {};

	bool build(std::shared_ptr<BuildingData> buildingData, sf::Vector2i position);
	bool build(unsigned int buildingID, sf::Vector2i position);
	bool destroy(sf::Vector2i position);

	std::vector<std::shared_ptr<BuildingData>> getBuildingData();

private:
	Game* game_;
	std::vector<std::shared_ptr<BuildingData>> buildingData_;
};