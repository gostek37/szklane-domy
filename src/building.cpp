#include "building.h"
#include "game-object.h"

Building::~Building() {
	std::cout << "Destroying building!!\n";
}

std::string Building::getTextureName() const {
	return data->textureName;
}
