#include "game.h"
#include "player.h"

Game::Game(): cityData(5000, 0), fpsCounter(0), frames(1), commandHistory(), player(nullptr) {
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;

    window = sf::Window(sf::VideoMode({800, 600}), "Szklane domy", sf::State::Windowed, settings);
    glewInit();
    
    // Makes it so the current window becomes the opengl context
    window.setActive();
    glViewport(0, 0, 800, 600);

    renderer = Renderer(&window);
}

void Game::populateWorld(const unsigned int& size) {
    grid = std::make_shared<Grid>(Grid(this));
    grid->populateGrid(size);

    // for debug here
    tileComponentManager = std::make_unique<TileComponentManager>(this);
}

void Game::initialize() {
    eventSubject.addObserver(renderer.getCamera());
    inputManager = std::make_shared<InputManager>(this);
    eventSubject.addObserver(inputManager);

    constructionManager = std::make_unique<ConstructionManager>(this);

    player = new Player(this);
    player->changeState(PlayerStateType::ZONE);

 //   tileComponentManager = std::make_unique<TileComponentManager>(this);
}

void Game::loop() {
    while (/*window.isOpen()*/ true) {
        sf::Time deltaTime = deltaTimer.restart();
        fpsCounter += deltaTime.asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
            
            eventSubject.notify(*event);
        }

        glm::vec3 newMouseWorldPos = renderer.getWorldMousePos();
        mouseWorldPos = sf::Vector2f(newMouseWorldPos.x * Grid::TILE_SIZE, newMouseWorldPos.y * Grid::TILE_SIZE);
        // std::cout << grid->worldToGridInt(mouseWorldPos).x << ", " << grid->worldToGridInt(mouseWorldPos).y << "\n";

        frames++;
        if (fpsCounter > 1) {
            fpsCounter = 0;
            std::cout << "Fps: " << frames << '\n';
            frames = 1;

            // Debug
            std::cout << "People: " << ++cityData.people << "\n";
        }

        player->currentState->update(deltaTime.asSeconds());
        // will have to gather all managers into an array and loop over them
        tileComponentManager->update(deltaTime.asSeconds());
        inputManager->update();

        renderer.draw(deltaTime.asSeconds());
    }
}