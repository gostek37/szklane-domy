#pragma once
#include <SFML/Graphics.hpp>

/**
* \brief Classic observer pattern
*/
class EventObserver : public std::enable_shared_from_this<EventObserver> {
public:
	virtual void onNotify(const sf::Event& event) = 0;
};