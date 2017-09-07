
#ifndef MJ_STATE_H__
#define MJ_STATE_H__

enum GameState
{
	GameState_Idle							= 0,
	GameState_BeforeGameStart				= 1,
	GameState_BeforeDispatch				= 2,
	GameState_BeforeOpWait					= 3,
	GameState_StandingCards					= 4,
	GameState_OpWait						= 5,
	GameState_GameStart						= 6,
	GameState_GameIng						= 7,
	GameState_Settlement					= 8,
	GameState_SettlementShow				= 9,
	GameState_GameIngIdle					= 10,
};

class MjState;
class MjStateListener
{
public:
	MjStateListener(){};
	virtual ~MjStateListener(){};

public:
	// Function to later add a new state to the manager
	virtual void manageState(GameState stateName, MjState *state) = 0;

	virtual MjState*	findByName(GameState stateName) = 0;

	// Exits the current app state and starts the one specified as the parameter
	virtual void	changeState(MjState *state) = 0;

	// Puts a new app state on the active state stack that will then be excecuted
	virtual bool	pushState(MjState* state) = 0;

	// Removes the top active state from the stack,
	// which results in returning to the one below
	virtual void	popState() = 0;

	// Pause the state
	virtual void	pauseState() = 0;

	// shutdown
	virtual void	shutdown() = 0;

	// Removes all current app states from the stack and moves to the given new state
	virtual void	popAllAndPushState(MjState* state) = 0;
};

class MjState{
public:
	static void create(MjStateListener* parent, const GameState name){};

	void destroy(){delete this;}

public:
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return true;}
	virtual void resume(){};
	virtual void update() = 0;

protected:
	MjState(){};

	MjState*	findByName(GameState stateName){return m_pParent->findByName(stateName);}
	void		changeState(MjState* state){m_pParent->changeState(state);}
	bool		pushState(MjState* state){return m_pParent->pushState(state);}
	void		popState(){m_pParent->popState();}
	void		shutdown(){m_pParent->shutdown();}
	void        popAllAndPushMjState(MjState* state){m_pParent->popAllAndPushState(state);}

	MjStateListener*	m_pParent;

private:
};


//|||||||||||||||||||||||||||||||||||||||||||||||
#define DECLARE_STATE_CLASS(T)					\
static void create(MjStateListener* parent, const GameState name)	\
{									\
	T* myAppState = new T();					\
	myAppState->m_pParent = parent;					\
	parent->manageState(name, myAppState);			\
}
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif // MJ_STATE_H__
