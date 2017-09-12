
#include "StateGaming.hpp"

StateGaming::StateGaming()
{
    m_bQuit         = false;
}

void StateGamin::enter()
{
}

void StateGaming::exit()
{
}

void StateGaming::update(double timeSinceLastFrame)
{
    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}