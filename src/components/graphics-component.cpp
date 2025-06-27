#include "graphics-component.h"
#include "grid.h"
#include <SFML/Window.hpp>

void GraphicsComponent::calculate_(TextureData& data) {
	auto owner = owner_.lock();
	auto scale = owner->getScale();
	auto position = owner->getPosition();
	float textureSize = (float)Grid::TILE_SIZE;

	float depthOffset = 0.f;
	// TODO properly xdd
	if (data.type == TextureType::OBJECT) {
		depthOffset = 1.0f;
		depthOffset /= (data.dimensions.x + data.dimensions.y) / 2;
	}

	glm::vec3 newOffset(position.x / Grid::TILE_SIZE, position.y / Grid::TILE_SIZE, position.y * -0.02f + depthOffset);
	data.instanceData[id_ * 2] = newOffset;
	data.instanceData[id_ * 2 + 1] = colour_;

	glBindVertexArray(data.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data.instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, id_ * sizeof(float) * 6, sizeof(float) * 3, &newOffset);
	glBufferSubData(GL_ARRAY_BUFFER, id_ * sizeof(float) * 6 + sizeof(float) * 3, sizeof(float) * 3, &colour_);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	dirty_ = false;
}

void GraphicsComponent::makeDirty() {
	dirty_ = true;
}

void GraphicsComponent::setId(const int& id) {
	id_ = id;
}

bool const GraphicsComponent::getDirty() {
	return dirty_;
}

void GraphicsComponent::receive_(const Message& message) {
	switch (message.type) {
	case (MessageType::MOVE_GRID):
		makeDirty();
		break;
	case (MessageType::TEXTURE_CHANGE):
		retexture_(std::get<std::string>(message.argument));
		break;
	case (MessageType::COMPONENT_REMOVAL):
		if (auto point = std::get_if<std::shared_ptr<Component>>(&message.argument)) {
			// Checks if this component is the one to be removed
			// All removable components should implement this, and the message sent by gameobject's removeComponent() should be the only way to remove a component
			// Don't need to check if the myself is this object, since there will always be only one component of certain type
			if (auto myself = std::dynamic_pointer_cast<GraphicsComponent>(*point)) {
				game_->renderer.removeComponent(shared_from_this());
			}
			std::cout << "Removing graphics component with texture " << textureName_ << "!\n";
		}
		break;
	case (MessageType::SET_COLOUR):
		recolour_(std::get<glm::vec3>(message.argument));
		break;
	case (MessageType::SET_DEFAULT_COLOUR): 
		defaultColour_ = std::get<glm::vec3>(message.argument);
		break;
	case (MessageType::RESET_COLOUR):
		recolour_(defaultColour_);
		break;
	default:
		break;
	}
}

void GraphicsComponent::initialize_() {
	game_->renderer.addComponent(shared_from_this());
}

void GraphicsComponent::recolour_(const glm::vec3& colour) {
	colour_ = colour;
	dirty_ = true;
}

void GraphicsComponent::retexture_(const std::string& texture) {
	// I can do it and the component won't get deleted, because it exists in the game object
	std::cout << "new: " << texture << '\n';
	game_->renderer.removeComponent(shared_from_this());
	game_->renderer.addComponent(shared_from_this(), texture);
}
