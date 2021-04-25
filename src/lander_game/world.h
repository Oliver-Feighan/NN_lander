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
    update(sf::RenderWindow & window);

    void
    generate_landscape();

    arma::mat
    get_lz();


    sf::Texture lander_texture;

    int n_points = 50;
    arma::mat landscape;
    Rocket rocket;
    sf::Clock clock;
    float current_time;

};

}

#endif // NN_LANDER_WORLD_H
