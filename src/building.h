#pragma once
#include "component.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "building-data.h"

class GameObject;

/**
* \brief Class representing a building.
*
* \details The Building class has a reference to its main owner, that it the main tile that owns this particular building. It also has an owner to the BuildingData
* struct, that holds information such as the buildings texture or size.
*/
class Building {
public:
	friend class GameObject;
	Building(GameObject* mainOwner, std::shared_ptr<BuildingData> data) : mainOwner(mainOwner), data(data) {};
	~Building();
	std::string getTextureName() const;

	GameObject* mainOwner;
	std::shared_ptr<BuildingData> data;
};