#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"
#include <vector>
#include <memory>
#include "event-observer.h"

/**
* \brief A class for managing the player's input.
*
* \details This class calls necessary methods when it receives events that an user input had been registered. Most notably, it calls the chosen playerState method,
* allowing the player to perform various actions.
*/
class InputManager: public EventObserver {
public:
	void update();
	InputManager(Game* game) : mousePos_(sf::Vector2f(0, 0)), mouseUpdated_(false), leftMousePressed_(false), rightMousePressed_(false), leftMouseDown_(false), rightMouseDown_(false), undone_(false), game_(game) {};
	
	void onNotify(const sf::Event& event) override;
private:
	void updateMouse_();
	void updateKeyboard_();

	sf::Vector2f mousePos_;
	bool mouseUpdated_;
	bool leftMousePressed_;
	bool rightMousePressed_;
	bool leftMouseDown_;
	bool rightMouseDown_;
	bool undone_;
	Game* game_;
};