#pragma once
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "game-object.h"
#include "components/component.h"
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "event-subject.h"
#include "input-manager.h"
#include "renderer.h"
#include "construction-manager.h"
#include "city-data.h"
#include "tile-component-manager.h"
#include "command-history.h"

class Grid;
class InputManager;
class ConstructionManager;
class TileComponentManager;
class Player;

/**
* \brief The class responsible for running all modules
*
* \details This is where all the managers, renderer, data ect are stored. Game initializes all of the required managers, and also runs the main Game loop,
* executing update methods on all objects that require it.
*/
class Game : public std::enable_shared_from_this<Game> {
public:
	//sf::RenderWindow window;
	sf::Window window;
	Renderer renderer;
	//std::shared_ptr<Camera> camera;
	EventSubject eventSubject;
	std::shared_ptr<Grid> grid;
	std::shared_ptr<InputManager> inputManager;
	std::unique_ptr<ConstructionManager> constructionManager;
	// standarize the managers!!
	std::unique_ptr<TileComponentManager> tileComponentManager;
	CityData cityData;
	CommandHistory commandHistory;
	Player* player;

	sf::Clock deltaTimer;
	float fpsCounter;
	unsigned int frames;

	sf::Vector2f mouseWorldPos;

	//Game() : window(sf::VideoMode({ 1920u, 1080u }), "Szklane Domy"), renderer(&window), camera(std::make_shared<Camera>(window)), eventSubject(), frames(1) {};
	Game();
	void initialize();
	void populateWorld(const unsigned int& size);
	void loop();
};