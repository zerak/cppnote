
#include "StateIdel.hpp"
#include "StateStart.hpp"
#include "AppStateManager.hpp"

class Demo
{
public:
    Demo(){m_pStateMgr = 0;}
    ~Demo()
    {
        if (m_pStateMgr != nullptr)
        {
            delete m_pStateMgr;
            m_pStateMgr = nullptr;
        }
    }

public:
    void init()
    {
        m_pStateMgr = new AppStateManager();

        StateIdel::create(stateMgr,GAME_STATE_IDEL);
        StateStart::create(stateMgr,GAME_STATE_START);
        StateGaming::create(stateMgr,GAME_STATE_GAMING);
    }

    void Start()
    {
        stateMgr->start(stateMgr->findByName(GAME_STATE_IDEL));
    }

private:
    AppStateManager*    m_pStateMgr;
};

int main()
{
}
