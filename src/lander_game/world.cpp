#include "world.h"

#include <armadillo>

namespace game {

void
World::generate_landscape(){

  this->landscape = arma::zeros(2, 2 * this->n_points);
  this->slopes = arma::zeros(this->n_points-1);

  for(int i = 0; i < this->landscape.n_cols; i+=2){
    int top = i;
    int bottom = top + 1;

    //x at the same position
    this->landscape.col(top).row(0) = top * 1800/this->n_points;
    this->landscape.col(bottom).row(0) = top * 1800/this->n_points;

    //y at a random position, top and bottom 50 pixels apart
    const auto y_pos = 1500 + 100 * (2*arma::randu()-1);
    this->landscape.col(top).row(1) = y_pos - 50;
    this->landscape.col(bottom).row(1) = 1800;

    if (i > 0 && i < 52){
      const arma::vec left = this->landscape.col(i-2);
      const arma::vec right = this->landscape.col(i);

      const auto dx = right(0) - left(0);
      const auto dy = right(1) - left(1);
      const auto hyp = std::pow(std::pow(dx, 2) + std::pow(dy, 2), 0.5);

      this->slopes(i-2) = std::asin(dy/hyp) * 180/arma::datum::pi;

    }
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

std::pair<arma::mat, float>
World::get_lz_and_slope(){

  const auto x_pos = this->rocket.getPosition().x;

  const auto index = 2*floor(n_points/2 * x_pos/1800);

  return {this->landscape.cols(index, index+3), this->slopes(index)};

}

sf::Vector2f
World::get_lowest_point(){

  sf::Vector2f result = {0., 0.};

  const std::vector<sf::Vector2f> y_points = {
    this->rocket.getTransform().transformPoint(0, 0),
    this->rocket.getTransform().transformPoint(150, 0),
    this->rocket.getTransform().transformPoint(0, 150),
    this->rocket.getTransform().transformPoint(150, 150)
  };

  for(const auto & y_point : y_points){
    if(y_point.y > result.y){
      result = y_point;
    }
  }

  return result;

}

void
World::check_landed(){

  const auto [lz, slope] = this->get_lz_and_slope();
  const auto lowest_point = this->get_lowest_point();

  const arma::vec first = lz.col(0);
  const arma::vec second = lz.col(2);

  const float dx = arma::accu(first.row(0) - second.row(0));
  const float dy = arma::accu(first.row(1) - second.row(1));

  const float m = dy/dx;
  const float c = arma::accu(first.row(1)) - m * arma::accu(first.row(0));

  const auto pos_x = this->rocket.getPosition().x;

  const float world_y = m * pos_x + c;

  this->landed = (world_y < lowest_point.y);
  this->distance_to_ground = std::max(world_y - lowest_point.y, 0.f);

}

void
World::reinitialize() {
  this->current_time = this->clock.getElapsedTime().asSeconds();

  this->n_points = 50;
  this->generate_landscape();

  this->rocket = Rocket();
  this->rocket.scale(0.4, 0.4);
  this->rocket.setTexture(this->lander_texture);

  this->landed = false;
}

void
World::print_info(
    sf::RenderWindow &window
    ){

  sf::Font font;
  font.loadFromFile("fonts/Courier_New_Bold.ttf");

  const auto pos = this->rocket.getPosition();

  const auto make_text = [&font](const std::string &str, const std::string &val) -> sf::Text{
    sf::Text result;
    result.setFont(font);
    result.setString(str + " : " + val);
    result.setFillColor(sf::Color::Black);
    return result;
  };

  const auto [lz, slope_val] = this->get_lz_and_slope();

  auto altitude   = make_text("altitude", std::to_string(this->distance_to_ground));
  auto x_velocity = make_text("x m/s", std::to_string(this->rocket.velocity.x));
  auto y_velocity = make_text("y m/s", std::to_string(this->rocket.velocity.y));
  auto slope      = make_text("slope", std::to_string(slope_val));

  const auto tilt_val = this->rocket.getRotation() < 180
                            ? this->rocket.getRotation()
                            : -(360 - this->rocket.getRotation());

  auto tilt       = make_text("tilt", std::to_string(tilt_val));

  altitude.setPosition((sf::Vector2f{10., 0.}));
  x_velocity.setPosition(sf::Vector2f{10., 35.});
  y_velocity.setPosition(sf::Vector2f{10., 70.});
  slope.setPosition(sf::Vector2f{10., 105.});
  tilt.setPosition(sf::Vector2f{10., 140.});

  window.draw(altitude);
  window.draw(x_velocity);
  window.draw(y_velocity);
  window.draw(slope);
  window.draw(tilt);


}

void
World::update(
    sf::RenderWindow & window
    ) {

  const auto dt = this->clock.getElapsedTime().asSeconds() - this->current_time;
  this->current_time = this->clock.getElapsedTime().asSeconds();

  const auto pos = this->rocket.getPosition();

  const auto [lz_mat, slope] = this->get_lz_and_slope();
  const auto lz = to_vertices(lz_mat, sf::Color::Red);
  this->check_landed();

  if(!this->landed){
    this->rocket.turn();
    this->rocket.accelerate();
    this->rocket.update_velocity(dt);
    this->rocket.update_position(dt);
  }

  window.draw(to_vertices(this->landscape));
  window.draw(lz);
  window.draw(this->rocket);

}

World::World(){

    this->lander_texture.loadFromFile("images/lander.png");
    this->current_time = this->clock.getElapsedTime().asSeconds();

    this->n_points = 50;
    this->generate_landscape();

    this->rocket = Rocket();
    this->rocket.scale(0.4, 0.4);
    this->rocket.setTexture(this->lander_texture);

    this->landed = false;

};

}
