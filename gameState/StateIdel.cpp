
#include "StateIdel.hpp"

StateIdel::StateIdel()
{
    m_bQuit         = false;
}

void StateIdel::enter()
{
    // some init
    createScene();
}

void StateIdel::createScene()
{
}

void StateIdel::exit()
{
}

void StateIdel::update(double timeSinceLastFrame)
{
    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}