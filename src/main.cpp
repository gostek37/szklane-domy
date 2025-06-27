#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "game.h"

int main() {
	Game game = Game();
	game.initialize();
	game.populateWorld(128);
	game.loop();
}