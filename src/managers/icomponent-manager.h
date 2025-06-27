#pragma once
#include <vector>
#include <memory>

/** 
* \brief Interface for Component Managers
*/
template<typename T>
class IComponentManager {
public:
	virtual void update(float deltaTime) = 0;
	std::vector<std::shared_ptr<T>> components;
};
