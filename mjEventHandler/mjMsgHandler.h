
#ifndef MJ_MSGHANDLER_H__
#define MJ_MSGHANDLER_H__

#include "mjCmdNo.h"

class Packet;
class GamePlayer;
class MsgHandler {
public:
    ~MsgHandler(){}

public:
    virtual void processMsg (Packet *pPacket, GamePlayer *pPlayer) = 0;
};

class MsgHandlerMgr {
public:
    MsgHandlerMgr();
    virtual ~MsgHandlerMgr();

public:
    int init();
    int exit();

    MsgHandler *getHandler(CmdNo msgType, MsgHandler *handler);

private:
    typedef map<CmdNo, MsgHandler*> MsgHandlerMap;
    typedef map<CmdNo, MsgHandler*>::iterator MsgHandlerIte;

    MsgHandlerMap m_Handlers;
};

#endif  // MJ_MSGHANDLER_H__
