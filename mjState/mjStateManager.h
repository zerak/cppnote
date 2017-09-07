
#ifndef MJ_STATE_MANAGER_H__
#define MJ_STATE_MANAGER_H__

#include <vector>
#include "mjState.h"

class MjStateManager : public MjStateListener
{
public:
	typedef struct
	{
		GameState name;
		MjState* state;
	} state_info;

	MjStateManager();
	~MjStateManager();

public:
	void manageState(GameState stateName, MjState* state);

	MjState* findByName(GameState stateName);

	void start(MjState* state);
	void changeState(MjState* state);
	bool pushState(MjState* state);
	void popState();
	void pauseState();
	void shutdown();
	void popAllAndPushState(MjState* state);

	void loop();

protected:
	void init(MjState *state);

protected:
	std::vector<MjState*>		m_ActiveStateStack;
	std::vector<state_info>		m_States;

	bool	m_bShutdown;
};

#endif // MJ_STATE_MANAGER_H__
