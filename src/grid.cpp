#include "grid.h"
#include "tile-component.h"
#include "hover-component.h"

Grid::Grid(Game* game) : game_(game), size_(0) {};

void Grid::populateGrid(unsigned int size) {
	size_ = size;
	for (int i = 0; i < size_; i++) {
		std::vector < std::list<std::shared_ptr<GameObject>>> newVector;
		for (int j = 0; j < size_; j++) {
			newVector.push_back(std::list<std::shared_ptr<GameObject>>());
		}
		gridObjects_.push_back(newVector);
	}

	for (int i = 0; i < size_; i++) {
			for (int j = 0; j < size_; j++) {
				sf::Vector2f objectPos((i + j) * TILE_SIZE / 2, (j - i + size_ - 1) * TILE_SIZE / 4);
				sf::Vector2i gridPos = worldToGridInt(objectPos);
				std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(objectPos, sf::Vector2f(1, 1), shared_from_this(), game_);
				gridObjects_.at(gridPos.y).at(gridPos.x).push_back(newObject);
				newObject->addComponent(std::make_shared<GraphicsComponent>(game_, "test-tile"));
				// newObject->addComponent(std::make_shared<InputComponent>(game_));
				newObject->addComponent(std::make_shared<TileComponent>(game_, TileType::GROUND));
				newObject->addComponent(std::make_shared<HoverComponent>());
			}
	}
}

std::list<std::shared_ptr<GameObject>>& Grid::getObjectsAt(const sf::Vector2i& position) {
	return gridObjects_.at(position.y).at(position.x);
}

void Grid::addObject(std::shared_ptr<GameObject> object) {
	sf::Vector2i pos = gridPositionClamped(worldToGridInt(object->getPosition()));
	gridObjects_.at(pos.y).at(pos.x).push_back(object);
}

void Grid::removeObject(std::shared_ptr<GameObject> object) {
	sf::Vector2i pos = gridPositionClamped(worldToGridInt(object->getPosition()));
	auto& list = getObjectsAt(pos);
	for (auto it = list.begin(); it != list.end(); it++) {
		if (object == *it) {
			gridObjects_.at(pos.y).at(pos.x).erase(it);
			// doesn't remove multiple instances of the same object, but makes the method a little bit faster
			return;
		}
	}
}

std::vector<std::shared_ptr<GameObject>> Grid::getTilesInSquare(const sf::Vector2i& position, const sf::Vector2i size) {
	std::vector<std::shared_ptr<GameObject>> objects;

	for (int y = position.y; y < std::min(position.y + size.y, static_cast<int>(size_)); y++) {
		for (int x = position.x; x < std::min(position.x + size.x, static_cast<int>(size_)); x++) {
			// returns the first object since the tile is always first
			objects.push_back(gridObjects_.at(y).at(x).front());
		}
	}

	return objects;
}

unsigned int Grid::getSize() const {
	return size_;
}

sf::Vector2i Grid::gridPositionClamped(sf::Vector2i orginal) {
	return sf::Vector2i(
		std::clamp(orginal.x, 0, static_cast<int>(size_) - 1),
		std::clamp(orginal.y, 0, static_cast<int>(size_) - 1)
	);
}

sf::Vector2i Grid::getMouseClamped() {
	sf::Vector2i mouseGridPos = worldToGridInt(game_->mouseWorldPos);
	return gridPositionClamped(mouseGridPos);
}

