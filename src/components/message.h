#pragma once
#include <variant>
#include <string>
#include "glm/glm.hpp"
#include <SFML/Window.hpp>

class Component;

using MessageArgument = std::variant<std::monostate, std::string, bool, std::shared_ptr<Component>, glm::vec3, sf::Vector2i>;

enum class MessageType {
	MOVE_GRID, MOUSE_HOVERED, TEXTURE_CHANGE, COMPONENT_ADDITION, COMPONENT_REMOVAL, SET_DEFAULT_COLOUR, SET_COLOUR, RESET_COLOUR
};

/**
* \brief A message that gets passed by the components
*
* \details The Message struct has only two fields: a MessageType enum and MessageArgument variant. Using such variant allows for the messages to contain one argument
* of variable type. For example, the MOUSE_HOVERED message will send out a bool to tell the object if the mouse enters or exits it, and the SET_COLOUR message
* will contain 3 floats representing Red Green and Blue colours. Such system allows for maximum decoupling of components.
*/
struct Message {
	MessageType type;
	MessageArgument argument;
	
	Message(MessageType type) : type(type), argument(std::monostate()) {};
	Message(MessageType type, std::string message) : type(type), argument(message) {};
	Message(MessageType type, bool value) : type(type), argument(value) {};
	Message(MessageType type, std::shared_ptr<Component> component) : type(type), argument(component) {};
	Message(MessageType type, glm::vec3 colour) : type(type), argument(colour) {};
	Message(MessageType type, sf::Vector2i pos) : type(type), argument(pos) {};
};

