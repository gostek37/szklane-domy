#include "player.h"

void Player::changeState(const PlayerStateType& state) {
	if (currentState) {
		currentState->optOut();
	}

	switch (state) {
	case PlayerStateType::BUILD:
		currentState = &buildState;
		break;
	case PlayerStateType::ZONE:
		currentState = &zoneState;
		break;
	default:
		break;
	}

	currentState->optIn();
}