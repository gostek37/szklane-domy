#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "stb-image.h"
#include <glm/glm.hpp>

class GraphicsComponent;

enum class TextureType {
	FLOOR, OBJECT
};

struct TextureData {
	int width;
	int height;
	int nrChannels;
	unsigned char* data;
	unsigned int textureId;
	unsigned int activeInstances;
	unsigned int instanceBufferSize;
	std::vector<std::shared_ptr<GraphicsComponent>> components;
	std::vector<glm::vec3> instanceData;
	// Grid dimension, such as 1x1, 2x2 ect
	glm::vec3 dimensions;
	// Origin can either be "bottom" or "centre". Centre is used for tiles, while bottom for everything else. This is later translated into the origin in pixels.
	// It is two dimensional, but it's a vec3 for easier translation
	glm::vec3 origin;

	// Set to NULL for purpose, so that nothing will use them without assignment
	unsigned int VBO;
	unsigned int VEO;
	unsigned int VAO;
	unsigned int instanceVBO;

	TextureType type;
	
	TextureData(const std::string& textureName, const glm::vec3& dimensions, const glm::vec3& origin, const TextureType& type) :
		width(0),
		height(0),
		nrChannels(0),
		data(nullptr),
		textureId(0),
		activeInstances(0),
		instanceBufferSize(0),
		components(),
		instanceData(),
		dimensions(dimensions),
		origin(origin),
		VBO(NULL),
		VEO(NULL),
		VAO(NULL),
		instanceVBO(NULL),
		type(type)
	{};
};

