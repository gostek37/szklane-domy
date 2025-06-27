#pragma once
#include <iostream>
#include <SFML/Window.hpp>
#include <map>

enum class BuildingType {
	UTILITY, HOUSING
};

/**
* \brief Struct holding buildings data, that later gets referenced by buildings. BuildingData is shared between all buildings.
*
* \details Some of the data it holds includes:
* - number of people it can fit, or, if an utility building, number of jobs it creates
* - the radius it affects
* - the variables and its minimum values the building requires to be on at least one of its tiles, in order to operate
* - if the building is weak
* - the buildings dimensions
* ect.
*/
struct BuildingData {
	std::string name;
	sf::Vector2i dimensions;
	std::string textureName;
	// number of people it can fit, or, if an utility building, number of jobs it creates
	unsigned int capacity;
	BuildingType type;
	// if an utility building, the radius it affects
	unsigned int radius;
	// the variables and its minimum values the building requires to be on at least one of its tiles, in order to operate
	std::map<std::string, double> requirements;
	// the variables the building affects
	std::map<std::string, double> affections;
	bool weak;


	BuildingData(const std::string& name, const sf::Vector2i& dimensions, const std::string& textureName, const unsigned int& capacity, BuildingType type, const unsigned int& radius,
		std::map<std::string, double> requirements, std::map<std::string, double> affections, bool weak):
		name(name), dimensions(dimensions), textureName(textureName), capacity(capacity), type(type), radius(radius), requirements(requirements), affections(affections), weak(weak) {};
};