#pragma once
#include "player-state.h"
#include "game-object.h"
#include <memory>

class Game;
class BuildingData;

/**
* \brief Implementation of IPlayerState that lets the player build.
*
* \details Player in this state can build buildings by pressing the left mouse button. The BuildingPlayerState stores the current building to build
* and a GameObject that moves with the cursor for previewing the map.
*/
class BuildingPlayerState : public IPlayerState {
public:
	BuildingPlayerState(Game* game) : game_(game), currentBuilding_(game_->constructionManager->getBuildingData().at(0)), previewObject_(nullptr) {};

	void mousePress(sf::Vector2i pos) override;
	void mouseRelease() override;
	void update(float deltaTime) override;
	void setOption(unsigned int option) override;

	void optIn() override;
	void optOut() override;

private:
	Game* game_;
	std::shared_ptr<BuildingData> currentBuilding_;
	std::shared_ptr<GameObject> previewObject_;
};