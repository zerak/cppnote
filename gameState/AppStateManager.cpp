
#include "AppStateManager.hpp"

AppStateManager::AppStateManager()
{
    m_bShutdown = false;
}

AppStateManager::~AppStateManager()
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

void AppStateManager::manageAppState(GameState stateName, AppState* state)
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

AppState* AppStateManager::findByName(GameState stateName)
{
    std::vector<state_info>::iterator itr;
    for(itr=m_States.begin();itr!=m_States.end();itr++)
    {
        if(itr->name==stateName)
            return itr->state;
    }

    return 0;
}

void AppStateManager::start(AppState* state)
{
    changeAppState(state);
}

void AppStateManager::changeAppState(AppState* state)
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

bool AppStateManager::pushAppState(AppState* state)
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

void AppStateManager::popAppState()
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

void AppStateManager::popAllAndPushAppState(AppState* state)
{
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    pushAppState(state);
}

void AppStateManager::pauseAppState()
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

void AppStateManager::shutdown()
{
    m_bShutdown = true;
}

void AppStateManager::init(AppState* state)
{
    // set state listener
}

void AppStateManager::update(double timeSinceLastFrame)
{
//    while (!m_bShutdown)
//    {
        m_ActiveStateStack.back().update(timeSinceLastFrame);
//    }
}