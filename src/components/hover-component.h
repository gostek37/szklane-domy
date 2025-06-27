#pragma once
#include "component.h"

/**
* \brief Component responsible for sending message to recolour, when it receives a message that the mouse if over the object. That's it.
*/
class HoverComponent: public Component {
public:
	friend class GameObject;

	HoverComponent() {};

private:
	void initialize_() override {};
	void receive_(const Message& message);
};