
#include "StateIdel.hpp"
#include "StateStart.hpp"
#include "AppStateManager.hpp"

#include <unistd.h>

class Demo
{
public:
    Demo(){m_pStateMgr = 0;}
    ~Demo()
    {
        delete m_pStateMgr;
        m_pStateMgr = 0;
    }

public:
    void init()
    {
        m_pStateMgr = new AppStateManager();

        StateIdel::create(stateMgr,GAME_STATE_IDEL);
        StateStart::create(stateMgr,GAME_STATE_START);
        StateGaming::create(stateMgr,GAME_STATE_GAMING);
    }

    void start()
    {
        m_pStateMgr->start(stateMgr->findByName(GAME_STATE_IDEL));
    }

    void update(double timeSinceLastFrame)
    {
       m_pStateMgr->update(timeSinceLastFrame);
    }

private:
    AppStateManager*    m_pStateMgr;
};

int main()
{
    Demo demo;
    demo.init();

    demo.Start();

    while (true)
    {
        demo.update(0);
        sleep(1);
    }

    return 0;
}
