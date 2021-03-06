#ifndef ROCKET_GAME_ROCKET_H
#define ROCKET_GAME_ROCKET_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Rocket: public sf::Sprite{

public:
  Rocket();

  sf::Vector2f forward = {0., -1.0};
  sf::Vector2f acceleration = {0., 0.};
  sf::Vector2f velocity = {0., 0};
  float rotation = 0.;

  bool
  accelerate(sf::Sound &engine_sound);

  bool
  turn(float dt);

  void
  update_position(float dt);

  void
  update_velocity(float dt);

  float thrust = 2.;
};


#endif//ROCKET_GAME_ROCKET_H