#include "event-subject.h"

void EventSubject::addObserver(std::shared_ptr<EventObserver> observer) {
	observers_.push_back(observer);
}

bool EventSubject::removeObserver(std::shared_ptr<EventObserver> observer) {
	for (auto& existing : observers_) {
		// I can do that because it doesnt compare shared_ptr objects, but dereferences them and compares the underlying pointers
		if (observer == existing) {
			return true;
		}
	}
	return false;
}

void EventSubject::notify(sf::Event event) {
	for (auto& observer : observers_) {
		observer->onNotify(event);
	}
}