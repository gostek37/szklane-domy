#include "tile-component.h"
#include "game-object.h"
#include "zone.h"

TileComponent::TileComponent(Game* game, const TileType& type) : variables(), mainBuildingOwner_(false), game_(game), type_(type) , zone_(nullptr), index(0) {
	for (auto& variable : game_->cityData.localVariables) {
		variables.insert({ variable.name, variable.value });
	}
};

bool TileComponent::isFreeSpace() const {
	if (building && !building->data->weak) {
		return false;
	}
	return true;
}

void TileComponent::addBuilding(std::shared_ptr<Building> newBuilding, bool mainOwner) {
	if (building) {
		removeBuilding();
	}
	building = newBuilding;
	mainBuildingOwner_ = mainOwner;
	if (mainBuildingOwner_) {
		auto owner = owner_.lock();
		owner->send(Message(MessageType::TEXTURE_CHANGE, building->getTextureName()));
		owner->send(Message(MessageType::SET_DEFAULT_COLOUR, glm::vec3{ 1, 1, 1 }));
		owner->send(Message(MessageType::RESET_COLOUR));
	} else {
		// removing unnecessary graphics components
		owner_.lock()->removeComponent(owner_.lock()->getComponent<GraphicsComponent>().get());
	}
}

void TileComponent::removeBuilding() {
	building = nullptr;
	if (mainBuildingOwner_) {
		mainBuildingOwner_ = false;
		owner_.lock()->send(Message(MessageType::TEXTURE_CHANGE, std::string("test-tile")));
		owner_.lock()->send(Message(MessageType::MOUSE_HOVERED, false));
	}
	else {
		owner_.lock()->addComponent(std::make_shared<GraphicsComponent>(owner_.lock()->game, "test-tile"));
	}

	if (zone_) {
		auto owner = owner_.lock();
		owner->send(Message(MessageType::SET_DEFAULT_COLOUR, zone_->colour));
		owner->send(Message(MessageType::RESET_COLOUR));
	}
}

bool TileComponent::setZone(Zone* zone) {
	if (zone != zone_) {
		zone_ = zone;
		return true;
	}
	return false;
}

Zone* TileComponent::getZone() {
	return zone_;
}

void TileComponent::receive_(const Message& message) {
	switch (message.type) {
	case MessageType::MOUSE_HOVERED:
		if (!mainBuildingOwner_ && building) {
			building->mainOwner->send(Message(MessageType::MOUSE_HOVERED, std::get<bool>(message.argument)));
		}
	}
}