
#include <unistd.h>
#include "mjDesk.h"
#include "mjStateManager.h"
#include "mjStateIdel.h"
#include "mjStateGaming.h"

MjDesk::MjDesk()
{
    m_StateMgr = 0;
}

MjDesk::~MjDesk()
{
    delete m_StateMgr;
    m_StateMgr = 0;
}

void MjDesk::start()
{
    m_StateMgr = new MjStateManager();

    MjStateIdel::create(m_StateMgr,GameState_Idle);
    MjStateGaming::create(m_StateMgr,GameState_GameIng);

    m_StateMgr->start(m_StateMgr->findByName(GameState_Idle));
}

void MjDesk::loop()
{
    m_StateMgr->loop();
}

//||||||||||||||||||||
// main entry
//||||||||||||||||||||
int main()
{
    MjDesk desk;
    desk.start();

    // loop
    while (true)
    {
        desk.loop();
        sleep(1);
    }

    return 0;
}
