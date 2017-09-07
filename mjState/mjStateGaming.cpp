
#include <iostream>
#include "mjStateGaming.h"

MjStateGaming::MjStateGaming()
{
}

void MjStateGaming::enter()
{
    std::cout<< "state gaming enter" << std::endl;
}

void MjStateGaming::exit()
{
    std::cout<< "state gaming exit" << std::endl;
}

void MjStateGaming::update()
{
    std::cout<< "state gaming update" << std::endl;
}
