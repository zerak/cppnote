
#include "mjDesk.h"
#include "mjStateManager.h"
#include "mjStateIdel.h"
#include "mjStateGaming.h"

MjDesk::MjDesk()
{
    m_StateMgr = new MjStateManager();
}

MjDesk::~MjDesk()
{
    delete m_StateMgr;
    m_StateMgr = nullptr;
}

void MjDesk::start()
{
    MjStateIdel::create(m_StateMgr,GameState_Idle);
    MjStateGaming::create(m_StateMgr,GameState_GameIng);

    m_StateMgr->start(m_StateMgr->findByName(GameState_Idle));
}

void MjDesk::loop()
{
    m_StateMgr->loop();
}

