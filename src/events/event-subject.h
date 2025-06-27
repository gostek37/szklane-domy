#pragma once
#include <SFML/Graphics.hpp>
#include "event-observer.h"
#include <memory>
#include <vector>

/**
* \brief Classic observer pattern
*/
class EventSubject {
public:
	void addObserver(std::shared_ptr<EventObserver> observer);
	bool removeObserver(std::shared_ptr<EventObserver> observer);
	std::vector<std::shared_ptr<EventObserver>> observers_;
	void notify(sf::Event event);
};