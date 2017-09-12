
#include "StateStart.hpp"
#include <iostream>

StateStart::StateStart()
{
}

void StateStart::enter()
{
    // some init
}

void StateStart::exit()
{
}

void StateStart::update(double timeSinceLastFrame)
{
    // state start update
    std::cout<<"state start update" << std::endl;

    // for test
    changeAppState(findByName(GAME_STATE_GAMING));
}