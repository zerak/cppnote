
#include <iostream>
#include "mjStateIdel.h"

MjStateIdel::MjStateIdel()
{
}

void MjStateIdel::enter()
{
   std::count<< "state idel enter" << std::endl;
}

void MjStateIdel::exit()
{
    std::count<< "state idel exit" << std::endl;
}

void MjStateIdel::update()
{
    std::count<< "state idel update" << std::endl;

    // do something
    // when need change to some other state
    changeState(findByName(GameState::GameState_GameIng));
}