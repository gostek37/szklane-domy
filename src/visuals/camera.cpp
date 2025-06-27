#include "camera.h"

void Camera::update(float deltaTime) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		velocity_.y = speed_;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		velocity_.y = -speed_;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		velocity_.x = -speed_;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		velocity_.x = speed_;
	}

	position_ += velocity_ * deltaTime;
	velocity_ -= velocity_ * 3.f * deltaTime;
}

glm::mat4 Camera::getView() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, -position_);
	return view;
}

float Camera::getZoom() const {
	return zoom_;
}

void Camera::onNotify(const sf::Event& event) {
	if (const auto scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
		// position_.z -= scroll->delta;
		// position_.z = std::max(position_.z, 1.f);
		zoom_ -= scroll->delta * 0.001f;
		zoom_ = std::max(zoom_, 0.01f);
	}
}
