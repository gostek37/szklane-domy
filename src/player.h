#pragma once
#include <SFML/Window.hpp>
#include "building-player-state.h"
#include "zone-add-player-state.h"
#include "player-state.h"
#include "vector"

enum class PlayerStateType {
	BUILD, ZONE
};

class Game;

/**
* \brief Stores various actions the player can do as strategies
*
* \details Can select the player state that later gets called by the input manager. The player states are easily interchangeable by the changeState method
*/

class Player {
public:
	BuildingPlayerState buildState;
	ZoneAddPlayerState zoneState;

	Player(Game* game) : buildState(game), zoneState(game), currentState(nullptr) {};

	void changeState(const PlayerStateType& state);

	IPlayerState* currentState;
};