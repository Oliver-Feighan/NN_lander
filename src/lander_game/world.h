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
    update(sf::RenderWindow & window);

    void
    print_info(sf::RenderWindow & window);

    void
    generate_landscape();

    void
    check_landed();

    sf::Vector2f
    get_lowest_point();

    arma::mat
    get_lz();

    sf::Texture lander_texture;
    int n_points;
    arma::mat landscape;
    Rocket rocket;
    sf::Clock clock;
    float current_time;
    bool landed;
    float distance_to_ground;

};

}

#endif // NN_LANDER_WORLD_H
