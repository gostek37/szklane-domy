#pragma once
#include "icomponent-manager.h"
#include "tile-component.h"
#include <memory>
#include <vector>
#include <zone.h>
#include "building-data.h"
// change into random engine
#include <random>

class Game;

/**
* \brief A manager of tile components
*
* \details This class stores and manages the Zones and tile components. This class also adds necessary housing when there are homeless people.
*/
class TileComponentManager : public IComponentManager<TileComponent> {
public:
	TileComponentManager(Game* game);

	bool addZone(std::vector<std::shared_ptr<GameObject>> tiles, unsigned int zoneID);

	void addFreeSpace(std::weak_ptr<TileComponent> tileComponent);
	void removeFreeSpace(std::weak_ptr<TileComponent> tileComponent);

	void update(float deltaTime) override;
	std::vector<std::shared_ptr<TileComponent>> components;
	std::vector<std::shared_ptr<Zone>> zones;
	Game* game;
};