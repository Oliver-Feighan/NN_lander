#include "game.h"
#include <SFML/Graphics.hpp>
#include "world.h"

namespace game {

int
lander_game(){

  sf::RenderWindow window(sf::VideoMode(1800, 1800), "NN Lander");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  auto world = World();
  bool paused = false;

  sf::SoundBuffer engine_sound_buffer;
  engine_sound_buffer.loadFromFile("sounds/engine.wav");

  sf::Sound engine_sound;
  engine_sound.setBuffer(engine_sound_buffer);

  while (window.isOpen()) {
    window.clear(sf::Color(255, 255, 255, 255));

    sf::Event event{};

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if( sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
      world.reinitialize();
      continue;
    }

    world.update(window, engine_sound);
    world.print_info(window);
    window.display();
  }

  return 0;
}

}
