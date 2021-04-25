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


    sf::Texture lander_texture;

    arma::vec landscape;
    Rocket rocket;
    sf::Clock clock;
    float current_time;

};

}

#endif // NN_LANDER_WORLD_H
