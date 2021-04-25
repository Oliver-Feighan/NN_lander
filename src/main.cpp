#include <iostream>

#include "lander_game/game.h"

int main(){

  std::cout << "###############" << std::endl;
  std::cout << "#  NN Lander  #" << std::endl;
  std::cout << "###############" << std::endl;

  try{

    game::lander_game();

  } catch (...){

  }

  return 0;

}