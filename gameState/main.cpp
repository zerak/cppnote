
#include "StateIdel.hpp"
#include "StateStart.hpp"
#include "StateGaming.hpp"
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

        StateIdel::create(m_pStateMgr,GAME_STATE_IDEL);
        StateStart::create(m_pStateMgr,GAME_STATE_START);
        StateGaming::create(m_pStateMgr,GAME_STATE_GAMING);
    }

    void start()
    {
        m_pStateMgr->start(m_pStateMgr->findByName(GAME_STATE_IDEL));
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
    demo.start();

    while (true)
    {
        demo.update(0);
        sleep(1);
    }

    return 0;
}
