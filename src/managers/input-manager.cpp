#include "input-manager.h"
#include "place-building-command.h"
#include "remove-building-command.h"
#include "construction-manager.h"
#include "player.h"

void InputManager::update() {
	updateMouse_();
	updateKeyboard_();
}

void InputManager::updateMouse_() {
	if (mouseUpdated_) {
		for (auto& object : game_->grid->getObjectsAt(game_->grid->worldToGridInt(mousePos_))) {
			object->send(Message(MessageType::MOUSE_HOVERED, false));
		}
	}

	sf::Vector2i mouseGridPos = game_->grid->worldToGridInt(game_->mouseWorldPos);
	unsigned int gridSize = game_->grid->getSize();
	if (mouseGridPos.x < 0 || mouseGridPos.y < 0 || mouseGridPos.x > gridSize - 1 || mouseGridPos.y > gridSize - 1) {
		leftMousePressed_ = false;
		rightMousePressed_ = false;
		return;
	}
	mouseUpdated_ = true;
	mousePos_ = game_->mouseWorldPos;

	for (auto& object : game_->grid->getObjectsAt(mouseGridPos)) {
		object->send(Message(MessageType::MOUSE_HOVERED, true));
		if (leftMousePressed_) {
			// debug only
			// game->commandHistory.runCommand(new PlaceBuildingCommand(3, mouseGridPos, game));
			game_->player->currentState->mousePress(mouseGridPos);
		}
		else if (rightMousePressed_) {
			// debug only
			game_->commandHistory.runCommand(new RemoveBuildingCommand(mouseGridPos, game_));
		}
	}

	leftMousePressed_ = false;
	rightMousePressed_ = false;
}

void InputManager::updateKeyboard_() {
	if (undone_) {
		game_->commandHistory.undo();
	}

	undone_ = false;
}

void InputManager::onNotify(const sf::Event& event) {
	if (const auto mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
		switch (mousePress->button) {
			case sf::Mouse::Button::Left:
				if (!leftMouseDown_) {
					leftMousePressed_ = true;
				}
				leftMouseDown_ = true;
				break;
			case sf::Mouse::Button::Right:
				if (!rightMouseDown_) {
					rightMousePressed_ = true;
				}
				rightMouseDown_ = true;
				break;
		}
	}
	if (const auto mouserelease = event.getIf<sf::Event::MouseButtonReleased>()) {
		switch (mouserelease->button) {
			case sf::Mouse::Button::Left:
				if (leftMouseDown_) {
					game_->player->currentState->mouseRelease();
				}
				leftMouseDown_ = false;
				break;
			case sf::Mouse::Button::Right:
				rightMouseDown_ = false;
				break;
		}
	}

	if (const auto key = event.getIf<sf::Event::KeyPressed>()) {
		switch (key->scancode) {
			case sf::Keyboard::Scancode::U:
				undone_ = true;
				break;
			case sf::Keyboard::Scancode::B:
				game_->player->changeState(PlayerStateType::BUILD);
				break;
			case sf::Keyboard::Scancode::Z:
				game_->player->changeState(PlayerStateType::ZONE);
				break;
			case sf::Keyboard::Scancode::M:
				std::cout << "KASA!!!$$$$$$$$\n";
				game_->cityData.money += 10000;
				break;
			case sf::Keyboard::Scancode::P:
				std::cout << "LUDZIE!!!!!\n";
				game_->cityData.people += 1000;
				break;
			case sf::Keyboard::Scancode::Num1:
				game_->player->currentState->setOption(1);
				break;
			case sf::Keyboard::Scancode::Num2:
				game_->player->currentState->setOption(2);
				break;
			case sf::Keyboard::Scancode::Num3:
				game_->player->currentState->setOption(3);
				break;
			case sf::Keyboard::Scancode::Num4:
				game_->player->currentState->setOption(4);
				break;
			case sf::Keyboard::Scancode::Num5:
				game_->player->currentState->setOption(5);
				break;
			case sf::Keyboard::Scancode::Num6:
				game_->player->currentState->setOption(6);
				break;
			case sf::Keyboard::Scancode::Num7:
				game_->player->currentState->setOption(7);
				break;
			case sf::Keyboard::Scancode::Num8:
				game_->player->currentState->setOption(8);
				break;
			case sf::Keyboard::Scancode::Num9:
				game_->player->currentState->setOption(9);
				break;
			default:
				break;
		}
	}
}