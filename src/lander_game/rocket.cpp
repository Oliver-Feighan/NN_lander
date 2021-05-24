#include "rocket.h"

#include <armadillo>

sf::Vector2f
true_forward(
    const sf::Vector2f &vec,
    const float angle){

  const auto rot = arma::mat{{std::cos(angle), -std::sin(angle)},
                             {std::sin(angle), std::cos(angle)}};

  const arma::vec res = rot * arma::vec{vec.x, vec.y};

  return {float(res(0)), float(res(1))};

}

Rocket::Rocket() {
  this->setOrigin(75., 75.);
  this->setPosition(900., 100);
}

bool
Rocket::accelerate(sf::Sound &engine_sound) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    const auto angle = this->getRotation() * arma::datum::pi / 180;
    this->acceleration += this->thrust * true_forward(this->forward, angle);

    if (engine_sound.getStatus() != sf::SoundSource::Playing) {
      //engine_sound.play();
    }

    return true;
  } else {
    this->acceleration = {0., 10.};

    if (engine_sound.getStatus() == sf::SoundSource::Playing) {
      //engine_sound.pause();
    }

    return false;
  }
}

bool
Rocket::turn(const float dt) {
  bool turned = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->rotation += -2.;
    turned = true;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    this->rotation += 2.;
    turned = true;
  }

  this->rotate(this->rotation * dt);

  return turned;
}

void
Rocket::update_velocity(const float dt){
  this->velocity += dt * this->acceleration;
}

void
Rocket::update_position(const float dt) {
  const auto position = this->getPosition();
  this->setPosition(position + dt * this->velocity);

}