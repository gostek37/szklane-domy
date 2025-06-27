#include "hover-component.h"
#include "game-object.h"
#include "glm/glm.hpp"

void HoverComponent::receive_(const Message& message) {
	switch (message.type) {
	case (MessageType::MOUSE_HOVERED):
		if (std::get<bool>(message.argument)) {
			owner_.lock()->send(Message(MessageType::SET_COLOUR, glm::vec3(0.5, 0.5, 0.5 )));
		}
		else {
			owner_.lock()->send(Message(MessageType::RESET_COLOUR));
		}
		break;
	default:
		break;
	}
}
