#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../game-object.h"
#include <memory>
#include "component.h"
#include "message.h"
#include "../game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture-data.h"


/**
* \brief Component responsible for looks
*
* \details GraphicsComponent is mainly responsible for calculating the quads dimensions and location. It has a dirty flag that gets set by either the Renderer
* or other Components, and gets recalculated only if its set, guaraanting that it doesn't perform any unnecessary calculations. It doesn't own its textures data,
* only modifies the one in the Renderer when necessary. By using OpenGls instanced arrays the GraphicsComponent can fairly easily change its own colour.
*/
class GraphicsComponent : public Component, public std::enable_shared_from_this<GraphicsComponent> {
public:
	friend class GameObject;
	friend class Renderer;

	GraphicsComponent(Game* game, const std::string& textureName) : game_(game), dirty_(true), id_(0), textureName_(textureName), colour_(glm::vec3(1.0, 1.0, 1.0)), defaultColour_(glm::vec3(1.0, 1.0, 1.0)), pixelOffset_(sf::Vector2i(0, 0)) {};

	void makeDirty();
	void setId(const int& id);
	bool const getDirty();
private:
	void initialize_() override;
	void receive_(const Message& message) override;

	void calculate_(TextureData& data);

	void recolour_(const glm::vec3& colour);
	void retexture_(const std::string& texture);

	Game* game_;
	// this flag is set when the vertices need to get recalculated, like when an object moves
	bool dirty_;
	int id_;
	std::string textureName_;
	glm::vec3 colour_;
	glm::vec3 defaultColour_;
	sf::Vector2i pixelOffset_;
};