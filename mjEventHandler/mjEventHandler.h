
#ifndef MJ_EVENTHANDLER_H__
#define MJ_EVENTHANDLER_H__

#include "mjCmdNo.h"

class Packet;
class GamePlayer;
class EventHandler {
public:
    ~EventHandler(){}

public:
    virtual void processEvent(Packet *pPacket, GamePlayer *pPlayer) = 0;
};

class EventHandlerMgr {
public:
    EventHandlerMgr();
    virtual ~EventHandlerMgr();

public:
    int init();
    int exit();

    EventHandler *getHandler(CmdNo msgType, EventHandler *handler);

private:
    typedef map<CmdNo, EventHandler*> EventHandlerMap;
    typedef map<CmdNo, EventHandler*>::iterator EventHandlerIte;

    EventHandlerMap m_Handlers;
};

#endif  // MJ_EVENTHANDLER_H__
