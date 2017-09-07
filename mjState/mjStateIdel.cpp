
#include <iostream>
#include "mjStateIdel.h"

MjStateIdel::MjStateIdel()
{
}

void MjStateIdel::enter()
{
   std::cout<< "state idel enter" << std::endl;
}

void MjStateIdel::exit()
{
    std::cout<< "state idel exit" << std::endl;
}

void MjStateIdel::update()
{
    std::cout<< "state idel update" << std::endl;

    // do something
    // when need change to some other state
    changeState(findByName(GameState_GameIng));
}