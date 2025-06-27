#pragma once
#include "game-object.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <list>

/** 
* \brief Class that stores the grid and every object, sorted by space they occupy mapped to the grid
* 
* \details The class also is responsible for constructing the world, returning a subset of it's objects and translating world space to grid space and vice-versa.
* It is a member of Game. The objects are stored as a vector of vectors of lists of shared pointers to objects. This is because the first vector - y, the second - x,
* and the list are all objects that could be occupying that particular space. The first object in the list will always be the tile.
*/

class Grid: public std::enable_shared_from_this<Grid> {
public:
	Grid() : size_(0), game_(nullptr) {};
	Grid(Game* game);

	void populateGrid(unsigned int size);

	std::list<std::shared_ptr<GameObject>>& getObjectsAt(const sf::Vector2i& position);
	void addObject(std::shared_ptr<GameObject> object);
	void removeObject(std::shared_ptr<GameObject> object);
	std::vector<std::shared_ptr<GameObject>> getTilesInSquare(const sf::Vector2i& position, const sf::Vector2i size);

	unsigned int getSize() const;

	// Had to do this this way, because I sometimes need a float returning an int and vice-versa
	// I know about if constexpr, but it kept messing up due to the floating-point imprecisions
	// And since its a template function, it needs to be defined entirely in the header file (when I defined it in .cpp the linker wouldn't link the files)
	template <typename T>
	sf::Vector2<int> worldToGridInt(sf::Vector2<T> org) {
		float halfW = TILE_SIZE / 2;
		float halfH = TILE_SIZE / 4;

		float gx = (org.x / halfW + org.y / halfH) / 2 - size_ / 2 + 1;
		float gy = (org.y / halfH - org.x / halfW) / 2 + size_ / 2;
		return sf::Vector2<int>((int)floor(gx), (int)floor(gy));
	}

	template <typename T>
	sf::Vector2<float> worldToGridFloat(sf::Vector2<T> org) {
		float halfW = TILE_SIZE / 2;
		float halfH = TILE_SIZE / 4;

		float gx = (org.x / halfW + org.y / halfH) / 2 - size_ / 2 + 1;
		float gy = (org.y / halfH - org.x / halfW) / 2 + size_ / 2;

		return sf::Vector2<float>(gx, gy);
	}

	template <typename T>
	sf::Vector2<float> gridToWorldFloat(sf::Vector2<T> g) {
		float halfW = TILE_SIZE / 2.0f;
		float halfH = TILE_SIZE / 4.0f;

		float A = static_cast<float>(g.x) + size_ * 0.5f - 1.0f;
		float B = static_cast<float>(g.y) - size_ * 0.5f;

		float worldX = halfW * (A - B);
		float worldY = halfH * (A + B);

		return { worldX, worldY };
	}

	template <typename T>
	sf::Vector2<int> gridToWorldInt(sf::Vector2<T> g) {
		auto wf = gridToWorldFloat(g);
		return {
			static_cast<int>(std::floor(wf.x)),
			static_cast<int>(std::floor(wf.y))
		};
	}

	static const int TILE_SIZE = 64;

	sf::Vector2i gridPositionClamped(sf::Vector2i orginal);
	sf::Vector2i getMouseClamped();

private:
	std::vector<std::vector<std::list<std::shared_ptr<GameObject>>>> gridObjects_;
	unsigned int size_;
	Game* game_;
};