#ifndef NN_LANDER_WORLD_H
#define NN_LANDER_WORLD_H

#include "rocket.h"

#include <armadillo>

namespace game {

class World {

  public :
    //constructor
    World();

    void
    reinitialize();

    void
    update(sf::RenderWindow & window,
           sf::Sound &engine_sound);

    void
    print_info(sf::RenderWindow & window);

    void
    generate_landscape();

    void
    check_landed();

    sf::Vector2f
    get_lowest_point();

    std::pair<arma::mat, float>
    get_lz_and_slope();

    sf::Texture lander_texture;

    int n_points;
    arma::mat landscape;
    arma::vec slopes;
    arma::mat best_lz;

    sf::Vector2f best_lz_pos;

    Rocket rocket;
    bool landed;
    float safe_speed = 50;
    float safe_tilt = 10;
    bool under_safe_speed;
    bool under_safe_tilt;
    bool over_lz;

    float distance_to_ground;

    sf::Clock clock;
    float current_time;

    float time_since_last_thrust = 0.;
    float time_since_last_turn = 0.;

    std::string log = "";
    bool written = false;


};

}

#endif // NN_LANDER_WORLD_H
