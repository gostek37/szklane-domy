#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "texture-data.h"
#include "stb-image.h"
#include "building-data.h"
#include "variable-data.h"
#include <memory>

class DataParser {
public:
	static std::map<std::string, TextureData> getTextureData();
	static std::vector<std::shared_ptr<BuildingData>> getBuildingData();
	static std::array<std::vector<VariableData>, 2> getVariableData();
};