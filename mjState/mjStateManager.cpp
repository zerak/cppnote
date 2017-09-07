
#include "mjStateManager.h"

MjStateManager::MjStateManager()
{
    m_bShutdown = false;
}

MjStateManager::~MjStateManager()
{
    state_info si;
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    while(!m_States.empty())
    {
        si = m_States.back();
        si.state->destroy();
        m_States.pop_back();
    }
}

void MjStateManager::manageState(GameState stateName, MjState* state)
{
    try
    {
        state_info new_state_info;
        new_state_info.name = stateName;
        new_state_info.state = state;
        m_States.push_back(new_state_info);
    }
    catch(std::exception& e)
    {
        delete state;
        throw "";
    }
}

MjState* MjStateManager::findByName(GameState stateName)
{
    std::vector<state_info>::iterator itr;
    for(itr=m_States.begin();itr!=m_States.end();itr++)
    {
        if(itr->name==stateName)
            return itr->state;
    }

    return 0;
}

void MjStateManager::changeState(MjState* state)
{
    if(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    m_ActiveStateStack.push_back(state);
    init(state);
    m_ActiveStateStack.back()->enter();
}

bool MjStateManager::pushState(MjState* state)
{
    if(!m_ActiveStateStack.empty())
    {
        if(!m_ActiveStateStack.back()->pause())
            return false;
    }

    m_ActiveStateStack.push_back(state);
    init(state);
    m_ActiveStateStack.back()->enter();

    return true;
}

void MjStateManager::popState()
{
    if(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    if(!m_ActiveStateStack.empty())
    {
        init(m_ActiveStateStack.back());
        m_ActiveStateStack.back()->resume();
    }
    else
    {
        shutdown();
    }
}

void MjStateManager::pauseState()
{
    if(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->pause();
    }

    if(m_ActiveStateStack.size() > 2)
    {
        init(m_ActiveStateStack.at(m_ActiveStateStack.size() - 2));
        m_ActiveStateStack.at(m_ActiveStateStack.size() - 2)->resume();
    }
}

void MjStateManager::shutdown()
{
    m_bShutdown = true;
}

void MjStateManager::popAllAndPushState(MjState* state)
{
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    pushState(state);
}

void MjStateManager::init(MjState* state)
{
    // set state to listener
}

void MjStateManager::start(MjState* state)
{
    changeState(state);
}

void MjStateManager::loop()
{
    while(!m_bShutdown)
    {
        m_ActiveStateStack.back()->update();
    }
}
