#include "game.h"
#include <SFML/Graphics.hpp>

#include "world.h"

namespace game {

int
lander_game(){

  sf::RenderWindow window(sf::VideoMode(1800, 1800), "NN Lander");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  World world;

  while (window.isOpen()) {
    window.clear(sf::Color(255, 255, 255, 255));

    sf::Event event{};

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    world.update(window);
    window.display();
  }

  return 0;
}

}
