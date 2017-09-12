
#include "StateIdel.hpp"
#include <iostream>

StateIdel::StateIdel()
{
}

void StateIdel::enter()
{
    // some init
}

void StateIdel::exit()
{
}

void StateIdel::update(double timeSinceLastFrame)
{
    // idel state udpate
    std::cout<<"state idel update" << std::endl;

    // for test
    changeAppState(findByName(GAME_STATE_START));
}