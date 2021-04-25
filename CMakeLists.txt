cmake_minimum_required(VERSION 3.17)
project(NN_lander)

set(CMAKE_CXX_STANDARD 20)

find_package(Armadillo REQUIRED)
find_package(SFML COMPONENTS window graphics system REQUIRED)

include_directories(${Catch2_DIR})
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${ARMADILLO_INCLUDE_DIRS})

add_executable(NN_lander
        src/main.cpp

        src/lander_game/game.cpp
        src/lander_game/world.cpp
        src/lander_game/rocket.cpp
)

target_link_libraries(NN_lander PRIVATE sfml-window)
target_link_libraries(NN_lander PRIVATE sfml-graphics)
target_link_libraries(NN_lander PRIVATE sfml-system)
target_link_libraries(NN_lander PRIVATE ${ARMADILLO_LIBRARIES})
