#pragma once
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include "texture-data.h"
#include <queue>
#include "stb-image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

// I hate this why do I have to do this the linker is so stupid >:-(
class GraphicsComponent;
class GameObject;

struct ComponentManagement {
	std::shared_ptr<GraphicsComponent> component;
	// if false, the component will be scheduled for removal
	bool create;
	std::string newTexture;

	ComponentManagement(std::shared_ptr<GraphicsComponent> component, bool create, std::string newTexture) : component(component), create(create), newTexture(newTexture) {};
	ComponentManagement(std::shared_ptr<GraphicsComponent> component, bool create) : component(component), create(create), newTexture("") {};
};

/**
* \brief Class responsible for rendering.
*
* \details Easily the most complicated part of the project. The Renderer class holds textureData, and each texture data holds a vector of graphics components
* and texture names. The Renderer class uses OpenGl's instanced rendering to render large amounts of components quickly, one texture at the time.
* The Renderer class initializes OpenGl, creates all of the required buffers, compiles shaders, loads all textures into the GPU.
* It has a vector of components, but doesn't calculate them all each frame, only ones that changed since last frame, gaining a lot of performance.
* The Renderer class also has a queuing system for graphicsComponents creation and removal, so that the components remove themselves only in one specified
* moment during the rendering process. This class also holds the Id's of OpenGl objects (VAOs, VEOs, VBOs) using TextureData.
* The Renderer also holds a Camera, which lets the player move the view.
*/
class Renderer {
private:
	// a map containing the name of the texture, the actual texture to a vertex array
	std::map <std::string, TextureData> textures_;

	sf::Window* window_;
	std::shared_ptr<Camera> camera_;

	unsigned int shaderProgram_;

	std::queue<ComponentManagement> componentsToModify_;
	void addComponent_(std::shared_ptr<GraphicsComponent> component, std::string textureName);
	void removeComponent_(std::shared_ptr<GraphicsComponent> component);
	void createShaders_();

	float time_;
public:
	friend class GraphicsComponent;

	Renderer() {};
	Renderer(sf::Window* window);

	std::shared_ptr<Camera> getCamera();

	glm::vec3 getWorldMousePos();

	void addComponent(std::shared_ptr<GraphicsComponent> component);
	void addComponent(std::shared_ptr<GraphicsComponent> component, const std::string& newTexture);
	void removeComponent(std::shared_ptr<GraphicsComponent> component);
	void draw(float deltaTime = 0);
};