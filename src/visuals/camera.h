#pragma once
#include <SFML/Graphics.hpp>
#include "event-observer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
* \brief A class letting the player move the view
*
* \details The Camera can zoom and move, it even has its own velocity to make its movements smoother. By using the getView method in the Camera the Renderer can
* retrieve necessary matrix to translate all objects into the world space.
*/
class Camera : public EventObserver {
public:
	Camera() : speed_(5.0f), velocity_(0.0f), position_(40.0f, 40.0f, 10.0f), zoom_(0.025) {};
	void update(float deltaTime);
	glm::mat4 getView();
	float getZoom() const;

	void onNotify(const sf::Event& event) override;
private:
	float speed_;
	glm::vec3 velocity_;
	glm::vec3 position_;
	float zoom_;
};