
#ifndef APP_STATE_MANAGER_HPP
#define APP_STATE_MANAGER_HPP

#include "AppState.hpp"
#include <vector>

class AppStateManager : public AppStateListener
{
public:
    typedef struct
    {
        GameState name;
        AppState* state;
    } state_info;

    AppStateManager();
    ~AppStateManager();

    void manageAppState(GameState stateName, AppState* state);

    AppState* findByName(GameState stateName);

    void start(AppState* state);
    void changeAppState(AppState* state);
    bool pushAppState(AppState* state);
    void popAppState();
    void pauseAppState();
    void shutdown();
    void popAllAndPushAppState(AppState* state);

    void update(double timeSinceLastFrame);

protected:
    void init(AppState *state);

    std::vector<AppState*>		m_ActiveStateStack;
    std::vector<state_info>		m_States;
    bool				        m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||