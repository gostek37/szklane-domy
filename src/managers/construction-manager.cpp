#include "construction-manager.h"
#include "game-object.h"
#include <algorithm>
#include "tile-component.h"
#include "tile-component-manager.h"

bool ConstructionManager::build(std::shared_ptr<BuildingData> buildingData, sf::Vector2i position) {
	std::vector<std::shared_ptr<TileComponent>> tiles;
	const auto& allObjects = game_->grid->getTilesInSquare(position, buildingData->dimensions);
	bool canPlace = true;

	for (auto& obj : allObjects) {
		tiles.push_back(obj->getComponent<TileComponent>());
		canPlace = canPlace && tiles.back()->isFreeSpace();
	}
	
	if (!canPlace) {
		std::cout << "Cannot place a building at: " << position.x << ", " << position.y << ".\n";
		return false;
	}

	GameObject* mainObject = game_->grid->getObjectsAt(position).front().get();
	TileComponentManager* tileCompManager = game_->tileComponentManager.get();

	bool first = true;
	auto building = std::make_shared<Building>(mainObject, buildingData);
	for (auto& tile : tiles) {
		if (tile->building) {
			tile->removeBuilding();
		}
		else {
			tileCompManager->removeFreeSpace(tile);
		}
		tile->addBuilding(building, first);
		first = false;
	}

	return true;
}

bool ConstructionManager::build(unsigned int buildingID, sf::Vector2i position) {
	return build(buildingData_[buildingID], position);
}

bool ConstructionManager::destroy(sf::Vector2i position) {
	std::shared_ptr<Building> building;
	if (!(building = game_->grid->getObjectsAt(position).front()->getComponent<TileComponent>()->building)) {
		std::cout << "No building to remove at " << position.x << ", " << position.y << ".\n";
		return false;
	}

	const auto& allObjects = game_->grid->getTilesInSquare(building->mainOwner->gridCoords, building->data->dimensions);

	TileComponentManager* tileCompManager = game_->tileComponentManager.get();
	for (auto& obj: allObjects) {
		auto tileComp = obj->getComponent<TileComponent>();
		tileComp->removeBuilding();
		tileCompManager->addFreeSpace(tileComp);
	}

	return true;
}

std::vector<std::shared_ptr<BuildingData>> ConstructionManager::getBuildingData() {
	return buildingData_;
}
