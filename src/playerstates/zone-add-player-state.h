#pragma once
#include "player-state.h"
#include <SFML/Graphics.hpp>

class Game;
class GameObject;

/**
* \brief Implementation of IPlayerState interface, allowing the player to add zones (for now there is only one)
*
* \details The ZoneAddPlayerState allows the player to drag out new zones. It stores the initial mouse click position and current one, allowing
* for highlighting rectangle fields for previewing the zone placement.
*/
class ZoneAddPlayerState : public IPlayerState {
public:
	ZoneAddPlayerState(Game* game) : game_(game), mousePressed_(false), initialPressPos_(), currentSelection_(), currentMouseGridPos_(), lastMouseGridPos_() {};

	void mousePress(sf::Vector2i pos) override;
	void mouseRelease() override;
	void update(float deltaTime) override;
	void setOption(unsigned int option) override {};

	virtual void optIn() override {};
	virtual void optOut() override;

private:
	Game* game_;
	bool mousePressed_;
	sf::Vector2i initialPressPos_;
	std::vector<std::shared_ptr<GameObject>> currentSelection_;
	sf::Vector2i currentMouseGridPos_;
	sf::Vector2i lastMouseGridPos_;

	void clearSelection();
	void changeSelection();
	// returns price of current selection (the zone is hardcoded for now)
	unsigned int getPrice();
};