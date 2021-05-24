#include "world.h"

#include <armadillo>
#include <iostream>
#include <fstream>

namespace game {

void
World::generate_landscape(){
  arma::arma_rng::set_seed_random();
  this->landscape = arma::zeros(2, 2 * this->n_points);
  this->slopes = arma::zeros(this->n_points-1);

  float best_slope = 90.;

  for(int i = 0; i < this->landscape.n_cols; i+=2){
    int top = i;
    int bottom = top + 1;

    //x at the same position
    this->landscape.col(top).row(0) = top * 1800./this->n_points;
    this->landscape.col(bottom).row(0) = top * 1800./this->n_points;

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

      auto current_slope = std::asin(dy/hyp) * 180/arma::datum::pi;
      this->slopes(i-2) = current_slope;

      if(std::abs(current_slope) < best_slope){
        best_slope = std::abs(current_slope);
        best_lz = arma::join_horiz(left, this->landscape.col(i-1), right, this->landscape.col(i+1));
        best_lz_pos.x = 0.5 * (left.at(0, 0) + right.at(0,0));
        best_lz_pos.y = 0.5 * (left.at(1,0) + right.at(1,0));

      }

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

  const auto index = 2*floor(n_points/2. * x_pos/1800);

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

  const auto world_y = this->best_lz_pos.y;

  this->landed = (world_y < lowest_point.y);
  this->distance_to_ground = std::max(world_y - lowest_point.y, 0.f);

  const auto vel = this->rocket.velocity;
  const auto speed = std::sqrt(std::pow(vel.x, 2) + std::pow(vel.y, 2));
  this->under_safe_speed = (speed < this->safe_speed);

  const auto tilt = this->rocket.getRotation() < 180
                        ? this->rocket.getRotation()
                        : -(360 - this->rocket.getRotation());

  const auto tilt_diff = std::abs(slope - tilt);
  this->under_safe_tilt = tilt_diff < this->safe_tilt;

  const auto x_diff =
      std::abs(this->rocket.getPosition().x - this->best_lz_pos.x);

  this->over_lz = (x_diff < 1800./this->n_points);
}

void
World::reinitialize() {
  this->landed = false;
  this->log = "";
  this->current_time = this->clock.getElapsedTime().asSeconds();

  this->n_points = 50;
  this->generate_landscape();

  this->rocket = Rocket();
  this->rocket.scale(0.4, 0.4);
  this->rocket.setTexture(this->lander_texture);

  this->written = false;
}

void
World::print_info(
    sf::RenderWindow &window
    ){

  sf::Font font;
  font.loadFromFile("fonts/Courier_New_Bold.ttf");

  const auto make_text = [&font](const std::string &str, const std::string &val) -> sf::Text{
    sf::Text result;
    result.setFont(font);
    result.setString(str + " : " + val);
    result.setFillColor(sf::Color::Black);
    return result;
  };

  const auto [lz, slope_val] = this->get_lz_and_slope();

  const auto tilt_val = this->rocket.getRotation() < 180
                        ? this->rocket.getRotation()
                        : -(360 - this->rocket.getRotation());

  const auto x_diff_val =this->rocket.getPosition().x - this->best_lz_pos.x;

  auto altitude   = make_text("altitude", std::to_string(this->distance_to_ground));
  auto x_diff     = make_text("distance", std::to_string(x_diff_val));
  auto x_velocity = make_text("x m/s", std::to_string(this->rocket.velocity.x));
  auto y_velocity = make_text("y m/s", std::to_string(this->rocket.velocity.y));
  auto slope      = make_text("slope", std::to_string(slope_val));
  auto tilt       = make_text("tilt", std::to_string(tilt_val));
  auto rotation   = make_text("rotation", std::to_string(this->rocket.rotation));

  altitude.setPosition((sf::Vector2f{10., 0.}));
  x_diff.setPosition(sf::Vector2f{10, 35.});
  x_velocity.setPosition(sf::Vector2f{10., 70.});
  y_velocity.setPosition(sf::Vector2f{10., 105.});
  slope.setPosition(sf::Vector2f{10., 140.});
  tilt.setPosition(sf::Vector2f{10., 175.});
  rotation.setPosition(sf::Vector2f{10., 210.});

  if(this->under_safe_speed){
    x_velocity.setFillColor(sf::Color::Green);
    y_velocity.setFillColor(sf::Color::Green);
  } else {
    x_velocity.setFillColor(sf::Color::Red);
    y_velocity.setFillColor(sf::Color::Red);
  }

  if(this->under_safe_tilt){
    tilt.setFillColor(sf::Color::Green);
  } else {
    tilt.setFillColor(sf::Color::Red);
  }

  if(this->over_lz){
    x_diff.setFillColor(sf::Color::Green);
  } else {
    x_diff.setFillColor(sf::Color::Red);
  }

  std::string instance_log = "";

  if (!this->landed) {
    instance_log += std::to_string(this->distance_to_ground) + " ";
    instance_log += std::to_string(x_diff_val) + " ";
    instance_log += std::to_string(this->over_lz) + " ";
    instance_log += std::to_string(this->rocket.velocity.x) + " ";
    instance_log += std::to_string(this->rocket.velocity.y) + " ";
    instance_log += std::to_string(this->under_safe_speed) + " ";
    instance_log += std::to_string(tilt_val) + " ";
    instance_log += std::to_string(this->under_safe_tilt) + " ";
    instance_log += std::to_string(this->rocket.rotation) + " ";
    instance_log += std::to_string(this->time_since_last_thrust) + " ";
    instance_log += std::to_string(this->time_since_last_turn) + " ";
    instance_log += std::to_string(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) + " ";
    instance_log += std::to_string(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) + " ";
    instance_log += std::to_string(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) + "\n";
    this->log += instance_log;
  }

  if (this->landed &&
      !this->written &&
      this->under_safe_tilt &&
      this->under_safe_speed &&
      this->over_lz) {

    this->written = true;

    std::ofstream log_file;
    log_file.open("NN_training/data.txt", std::ios_base::app);
    log_file << this->log;
    log_file.close();

  }



  window.draw(altitude);
  window.draw(x_diff);
  window.draw(x_velocity);
  window.draw(y_velocity);
  window.draw(slope);
  window.draw(tilt);
  window.draw(rotation);

}

void
World::update(
    sf::RenderWindow & window,
    sf::Sound & engine_sound
    ) {

  const float dt = this->clock.getElapsedTime().asSeconds() - this->current_time;
  this->current_time = this->clock.getElapsedTime().asSeconds();

  const auto [lz_mat, slope] = this->get_lz_and_slope();
  const auto lz = to_vertices(lz_mat, sf::Color::Red);
  this->check_landed();

  if(!this->landed){
    const auto turned = this->rocket.turn(dt);
    this->time_since_last_turn = turned ? 0. : this->time_since_last_turn + dt;
    const auto thrust = this->rocket.accelerate(engine_sound);
    this->time_since_last_thrust = thrust ? 0. : this->time_since_last_thrust + dt;

    this->rocket.update_velocity(dt);
    this->rocket.update_position(dt);
  }

  window.draw(to_vertices(this->landscape));
  window.draw(lz);
  window.draw(to_vertices(this->best_lz, sf::Color::Green));

  sf::CircleShape lz_point(20., 4);
  lz_point.setOrigin(20., 20.);
  lz_point.setPosition(this->best_lz_pos);
  lz_point.setFillColor(sf::Color::Red);

  window.draw(lz_point);
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
