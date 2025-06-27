#pragma once
#include "component.h"
#include "building.h"
#include <variant>
#include <map>
#include <string>

class Zone;

enum class TileType {
	GROUND, ROAD
};

/**
* \brief The component for making the object a tile
*
* \details The TileComponent has a pointer to the building that stands on top of it (it is a nullptr if empty), has necessary methods for building addition and
* removal, can change the tile's zone, sends messages to remove the graphicsComponents if they are unnecessary (occluded by the building on top of it), stores
* the variables of the tile (such as electricity, education, road access ect. (not fully implemented yet)). It also stores its own index, which is required
* by the TileComponentManager to sort all tileComponents correctly.
*/
class TileComponent : public Component {
public:
	friend class GameObject;
	friend class TileComponentManager;
	
	TileComponent(Game* game, const TileType& type);

	std::shared_ptr<Building> building;
	bool isFreeSpace() const;
	void addBuilding(std::shared_ptr<Building>, bool mainOwner = false);
	void removeBuilding();
	bool setZone(Zone* zone);
	Zone* getZone();
	unsigned int index;
	std::map<std::string, double> variables;
private:
	void initialize_() override {};
	void receive_(const Message& message);
	bool mainBuildingOwner_;
	Game* game_;
	Zone* zone_;

	TileType type_;
};
