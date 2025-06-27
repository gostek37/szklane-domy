#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>

class Game;

/**
* \brief An interface for player state
*
* \details Basically the strategy pattern. A playerState that inherits this interface has few methods that execute with the mouse.
*/
class IPlayerState {
public:
	virtual void mousePress(sf::Vector2i pos) = 0;
	virtual void mouseRelease() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void setOption(unsigned int option) = 0;

	// those methods get called when changing into the state and out of it
	virtual void optIn() = 0;
	virtual void optOut() = 0;

private:
	Game* game_;
};