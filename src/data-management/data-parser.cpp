#include "data-parser.h"
#include "grid.h"
#include <algorithm>

std::map<std::string, TextureData> DataParser::getTextureData() {
	std::ifstream file("data/texture-data.json");
	nlohmann::json data(nlohmann::json::parse(file));
	file.close();

	std::map<std::string, TextureData> texData = std::map<std::string, TextureData>();
	stbi_set_flip_vertically_on_load(true);

	for (const auto& item : data) {
		std::string name = item.at("name");
		std::cout << "Texture \"" << name << "\" loaded.\n";
		glm::vec3 dimensions(item.at("dimensions")[0], item.at("dimensions")[1], item.at("dimensions")[2]);
		glm::vec3 origin(0.0, 0.0, 0.0);
		if (item.at("origin") == "bottom") {
			// Will have to do something else for unconventional sizes
			origin.y = -0.5f * std::max(dimensions.x, dimensions.y) + 0.25f;
		}
		std::string typeStr = item.at("type");
		TextureType type;
		if (typeStr == "floor") {
			type = TextureType::FLOOR;
		}
		else if (typeStr == "object") {
			type = TextureType::OBJECT;
		}

		// Create vertex buffers and arrays
		unsigned int VBO, VEO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VEO);
		glGenVertexArrays(1, &VAO);

		texData.insert(std::make_pair(name, TextureData(name, dimensions, origin, type)));

		TextureData& newData = texData.at(name);
		std::string filename = "textures/" + name + ".png";
		newData.data = stbi_load(filename.c_str(), &newData.width, &newData.height, &newData.nrChannels, 0);
	}

	return texData;
}

std::vector<std::shared_ptr<BuildingData>> DataParser::getBuildingData() {
	std::ifstream file("data/building-data.json");
	nlohmann::json data(nlohmann::json::parse(file));
	file.close();

	// its a vector of pointers so that buildings pointing into it wont lose the connection if buildingData changes in the future
	std::vector<std::shared_ptr<BuildingData>> buildingData;

	for (const auto& item : data) {
		std::string name = item.at("name");
		sf::Vector2i dimensions = { item.at("dimensions")[0], item.at("dimensions")[1] };
		std::string textureName = item.at("texture");
		unsigned int capacity = item.at("capacity");
		BuildingType type;
		std::string stringType = item.at("type");
		if (stringType == "utility") {
			type = BuildingType::UTILITY;
		}
		else if (stringType == "housing") {
			type = BuildingType::HOUSING;
		}
		unsigned int radius = item.at("radius");
		std::map<std::string, double> requirements = item.value("requirements", nlohmann::json::object()).get<std::map<std::string, double>>();
		std::map<std::string, double> affections = item.value("affections", nlohmann::json::object()).get<std::map<std::string, double>>();
		bool weak = false;
		if (item.at("strength") == "weak") {
			weak = true;
		}

		buildingData.push_back(std::make_shared<BuildingData>(BuildingData(name, dimensions, textureName, capacity, type, radius, std::move(requirements), std::move(affections), weak)));
	}

	return buildingData;
}

std::array<std::vector<VariableData>, 2> DataParser::getVariableData() {
	std::ifstream file("data/variables-data.json");
	nlohmann::json data(nlohmann::json::parse(file));
	file.close();

	std::array<std::vector<VariableData>, 2> variableData;
	unsigned int current = 0;

	for (const auto& item : data) {
		std::string name = item.at("name");
		double defaultValue = item.at("default");
		std::string scope = item.at("scope");

		if (scope == "global") {
			current = 0;
		}
		else if (scope == "local") {
			current = 1;
		}
		else {
			// skips the wrong variable
			std::cerr << "Wrong variable data scope: " << name << '!\n';
			throw 0;
		}

		variableData[current].push_back(VariableData(name, defaultValue));
	}

	return variableData;
}
