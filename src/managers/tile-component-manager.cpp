#include "tile-component-manager.h"
#include "game.h"
#include <SFML/Main.hpp>
#include "data-parser.h"
#include <algorithm>

TileComponentManager::TileComponentManager(Game* game) : components(), zones(), game(game) {
	zones.push_back(std::make_shared<Zone>(Zone(ZoneType::LIVING, DataParser::getBuildingData()[0], glm::vec3(0.7, 0.7, 1.0), 5)));
}

bool TileComponentManager::addZone(std::vector<std::shared_ptr<GameObject>> tiles, unsigned int zoneID){
	for (auto& tileObj : tiles) {
		auto tileComponent = tileObj->getComponent<TileComponent>();
		// skips tiles that are added already
		if (!tileComponent->setZone(zones[zoneID].get())) {
			continue;
		}
		zones[zoneID]->tiles.push_back(tileComponent);
		tileComponent->index = zones[zoneID]->tiles.size() - 1;
		if (tileComponent->building) {
			removeFreeSpace(tileComponent);
		}
		else {
			tileObj->send(Message(MessageType::SET_DEFAULT_COLOUR, { zones[zoneID]->colour }));
			tileObj->send(Message(MessageType::RESET_COLOUR));
		}
	}
	return true;
}

void TileComponentManager::addFreeSpace(std::weak_ptr<TileComponent> tileComponent) {
	auto component = tileComponent.lock();
	auto zone = component->getZone();
	if (!zone) {
		return;
	}
	zone->freeTileIndex--;
	zone->tiles.at(zone->freeTileIndex).lock()->index = component->index;
	std::iter_swap(zone->tiles.begin() + zone->freeTileIndex, zone->tiles.begin() + component->index);
	component->index = zone->freeTileIndex;
}

void TileComponentManager::removeFreeSpace(std::weak_ptr<TileComponent> tileComponent) {
	auto component = tileComponent.lock();
	auto zone = component->getZone();
	if (!zone) {
		return;
	}
	// if full than freeTileIndex is pointing to the space after the vector, and so does the end() so i can just check their equality
	if (zone->tiles.begin() + zone->freeTileIndex != zone->tiles.end()) {
		zone->tiles.at(zone->freeTileIndex).lock()->index = component->index;
		std::iter_swap(zone->tiles.begin() + zone->freeTileIndex, zone->tiles.begin() + component->index);
		component->index = zone->freeTileIndex;
	}
	zone->freeTileIndex++;
}

void TileComponentManager::update(float deltaTime) {
	// living zone
	unsigned int freeTileCount = zones[0]->tiles.size() - zones[0]->freeTileIndex;
	if (freeTileCount > 0 && game->cityData.people > zones[0]->baseBuildingCount * zones[0]->baseBuilding->capacity) {
		// Build
		// TODO uncouple (add zone strategy, or better custom one read from json)
		sf::Vector2i newBuildingCoords = zones[0]->tiles[zones[0]->freeTileIndex + rand() % freeTileCount].lock()->owner_.lock()->gridCoords;
		game->constructionManager->build(zones[0]->baseBuilding, newBuildingCoords);
		zones[0]->baseBuildingCount++;
		std::cout << "Free spaces on 0: [" << freeTileCount << "/" << zones[0]->tiles.size() << "]\n";
	}
}

