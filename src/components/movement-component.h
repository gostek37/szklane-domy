#pragma once
#include "component.h"

/**
* \brief Component responsible for moving the GameObject, and sending the message that the object had moved.
*/
class MovementComponent : public Component {
public:
	friend class GameObject;

	MovementComponent(Game* game) : game_(game) {};

private:
	void initialize_() override {};
	void receive_(const Message& message);
	Game* game_;
};