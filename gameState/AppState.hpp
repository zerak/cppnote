
#ifndef APP_STATE_HPP
#define APP_STATE_HPP

enum GameState
{
	GAME_STATE_IDEL		= 0,
	GAME_STATE_START	= 1,
	GAME_STATE_GAMING	= 2,
};

class AppState;
class AppStateListener
{
public:
    AppStateListener(){};
    virtual ~AppStateListener(){};

public:
	// Function to later add a new state to the manager
    virtual void manageAppState(GameState stateName, AppState* state) = 0;

    virtual AppState*	findByName(GameState stateName) = 0;

	// Exits the current app state and starts the one specified as the parameter
    virtual void		changeAppState(AppState *state) = 0;

	// Puts a new app state on the active state stack that will then be excecuted
    virtual bool		pushAppState(AppState* state) = 0;

	// Removes the top active state from the stack,
	// which results in returning to the one belo
    virtual void		popAppState() = 0;

	// Pause the stat
    virtual void		pauseAppState() = 0;

	// shutdow
    virtual void		shutdown() = 0;

	// Removes all current app states from the stack and moves to the given new stat
    virtual void		popAllAndPushAppState(AppState* state) = 0;
};

class AppState
{
public:
    static void create(AppStateListener* parent, const GameState name){};

    void destroy(){delete this;}

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual bool pause(){return true;}
    virtual void resume(){};
    virtual void update(double timeSinceLastFrame) = 0;

protected:
    AppState(){};

    AppState*	findByName(GameState stateName){return m_pParent->findByName(stateName);}
    void		changeAppState(AppState* state){m_pParent->changeAppState(state);}
    bool		pushAppState(AppState* state){return m_pParent->pushAppState(state);}
    void		popAppState(){m_pParent->popAppState();}
    void		shutdown(){m_pParent->shutdown();}
    void        popAllAndPushAppState(AppState* state){m_pParent->popAllAndPushAppState(state);}

protected:
    AppStateListener*	m_pParent;

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#define DECLARE_APPSTATE_CLASS(T)					\
static void create(AppStateListener* parent, const GameState name)	\
{									\
	T* myAppState = new T();					\
	myAppState->m_pParent = parent;					\
	parent->manageAppState(name, myAppState);			\
}

#endif
