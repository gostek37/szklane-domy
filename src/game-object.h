#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "components/component.h"
#include "components/graphics-component.h"
#include <memory>
#include "vector"
#include "message.h"
#include "grid.h"

/**
* \brief Base Object containing components.
*
* \details Base Object containing components. All of it's functionality gets modelled by the components it holds. It allows the components to freely communicate
* between each other by providing a message method, that lets the components send the Message object. It also has a position and a grid position. Upon destruction it
* doesn't destroy its components, but just sends them a message asking for components to delete themselves, in order to defer the components deletion in time. (So
* for example, the renderComponent will delete itself before even starting to render the current frame, not during nor after. It will live in its own manager
* after GameObject's deletion).
*/
class GameObject: public sf::Transformable, public std::enable_shared_from_this<GameObject> {
public:
	friend class Component;

	GameObject(const sf::Vector2f& position, const sf::Vector2f& scale, std::shared_ptr<Grid> grid, Game* game);
	~GameObject();

	void addComponent(std::shared_ptr<Component> component);
	void removeComponent(Component* component);

	template <typename T>
	std::shared_ptr<T> getComponent() const {
		static_assert(std::is_base_of_v<Component, T>, "getComponent<T> only works if T is derived from component!!");

		for (auto& component : components_) {
			if (auto ptr = std::dynamic_pointer_cast<T>(component)) {
				return ptr;
			}
		}
		return nullptr;
	}

	void send(const Message& message);

	sf::Vector2i gridCoords;
	Game* game;
private:
	std::vector<std::shared_ptr<Component>> components_;
};