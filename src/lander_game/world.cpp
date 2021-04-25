#include "world.h"

#include <armadillo>

namespace game {

void
World::generate_landscape(){


  this->landscape = arma::zeros(2, 2 * this->n_points);

  for(int i = 0; i < this->landscape.n_cols; i+=2){
    int top = i;
    int bottom = top + 1;

    //x at the same position
    this->landscape.col(top).row(0) = top * 1800/this->n_points;
    this->landscape.col(bottom).row(0) = top * 1800/this->n_points;

    //y at a random position, top and bottom 50 pixels apart
    const auto y_pos = 1500 + 100 * (2*arma::randu()-1);
    this->landscape.col(top).row(1) = y_pos - 50;
    this->landscape.col(bottom).row(1) = y_pos + 50;

  }

}

sf::VertexArray
to_vertices(
    const arma::mat &landscape,
    const sf::Color color = sf::Color::Black
    ){

  sf::VertexArray vertices(sf::TriangleStrip, landscape.n_cols);
  for(int i = 0; i < vertices.getVertexCount(); i++){
    vertices[i].position = {float(landscape.col(i)(0)), float(landscape.col(i)(1))};
    vertices[i].color = color;
  }

  return vertices;
}

arma::mat
World::get_lz(){

  const auto x_pos = this->rocket.getPosition().x;

  const auto index = floor(n_points * x_pos/1800);

  return this->landscape.cols(index-1, index+2);

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

  const auto lz = this->get_lz();

  window.draw(to_vertices(this->landscape));
  window.draw(to_vertices(lz, sf::Color::Red));
  window.draw(this->rocket);

}

World::World(){

    this->lander_texture.loadFromFile("images/lander.png");
    this->current_time = this->clock.getElapsedTime().asSeconds();

    this->generate_landscape();

    this->rocket = Rocket();
    this->rocket.scale(0.4, 0.4);
    this->rocket.setTexture(this->lander_texture);

};

}
