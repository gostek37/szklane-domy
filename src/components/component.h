#pragma once
#include <iostream>
#include "message.h"
#include <memory>

class GameObject;
class Game;

/**
* \brief A base component class that gets inherited by all Components.
*
* \details This isn't a full-on inteface, since it has one implemented method -> setOwner. After the component gets added to its owner, the initialize_ method
* gets called. Components shape how everything in the game works. So wether the object can be seen, move, treated as a tile ect. The components can receive
* messages send to the GameObject that owns them either by other components or managers. Such communication allows for pretty decoupling, meaning that each
* component has no idea that other exist, but can still talk to them.
*/
class Component {
public:
	void setOwner(std::shared_ptr<GameObject> owner);

protected:
	friend class GameObject;
	virtual void initialize_() = 0;
	virtual void receive_(const Message& message) = 0;

	std::weak_ptr<GameObject> owner_;
	Game* game_;
};