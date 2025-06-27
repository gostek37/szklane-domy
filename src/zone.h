#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <tile-component.h>
#include <building-data.h>
#include "glm/glm.hpp"

enum class ZoneType {
	LIVING, INDUSTRIAL
};

// TODO remake into a json read file
/**
* \brief Stores various zone variables
* 
* \details Such variables include for ex. the type of the zone, it's price per tile and what is this zone's base building
*/
struct Zone {
	ZoneType zoneType;
	std::vector<std::weak_ptr<TileComponent>> tiles;
	std::shared_ptr<BuildingData> baseBuilding;
	unsigned int baseBuildingCount;
	// TODO add bounds for faster looping maybe?
	glm::vec3 colour;
	unsigned int price;
	// the tiles vector is split like this: [occupied tiles][freeTiles]. Below is an index of the first free tile
	unsigned int freeTileIndex;

	Zone(ZoneType type, std::shared_ptr<BuildingData> baseBuilding, glm::vec3 colour, unsigned int price) : 
		zoneType(type),
		baseBuilding(baseBuilding),
		tiles(),
		baseBuildingCount(0),
		colour(colour),
		price(price),
		freeTileIndex(0)
	{};
};