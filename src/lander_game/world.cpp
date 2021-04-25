#include "world.h"

#include <armadillo>

namespace game {

arma::vec
generate_landscape(){
  const auto temp = arma::randu(10);
  return arma::randu(10);
}

void
World::update(
    sf::RenderWindow & window
    ) {

  const auto dt = this->clock.getElapsedTime().asSeconds() - this->current_time;
  this->current_time = this->clock.getElapsedTime().asSeconds();

  this->rocket.turn();
  this->rocket.accelerate();
  this->rocket.update_velocity(dt);
  this->rocket.update_position(dt);

  sf::Vertex vertices[] =
      {
          sf::Vertex(sf::Vector2f(0, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(0, 800), sf::Color::Black),

          sf::Vertex(sf::Vector2f(180, 800), sf::Color::Black),
          sf::Vertex(sf::Vector2f(180, 700), sf::Color::Black),

          sf::Vertex(sf::Vector2f(360, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(360, 800), sf::Color::Black),

          sf::Vertex(sf::Vector2f(540, 800), sf::Color::Black),
          sf::Vertex(sf::Vector2f(540, 700), sf::Color::Black),

          sf::Vertex(sf::Vector2f(720, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(720, 800), sf::Color::Black),

          sf::Vertex(sf::Vector2f(900, 800), sf::Color::Black),
          sf::Vertex(sf::Vector2f(900, 700), sf::Color::Black),

          sf::Vertex(sf::Vector2f(1080, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(1080, 800), sf::Color::Black),

          sf::Vertex(sf::Vector2f(1260, 800), sf::Color::Black),
          sf::Vertex(sf::Vector2f(1260, 700), sf::Color::Black),

          sf::Vertex(sf::Vector2f(1440, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(1440, 800), sf::Color::Black),

          sf::Vertex(sf::Vector2f(1620, 800), sf::Color::Black),
          sf::Vertex(sf::Vector2f(1620, 700), sf::Color::Black),

          sf::Vertex(sf::Vector2f(1800, 900), sf::Color::Black),
          sf::Vertex(sf::Vector2f(1800, 800), sf::Color::Black)
      };

  window.draw(vertices, (sizeof(vertices)/sizeof(*vertices)), sf::TriangleStrip);

  window.draw(this->rocket);

}

World::World(){

    this->lander_texture.loadFromFile("images/lander.png");
    this->current_time = this->clock.getElapsedTime().asSeconds();

    this->landscape = generate_landscape();

    this->rocket = Rocket();
    this->rocket.setTexture(this->lander_texture);

};

}
