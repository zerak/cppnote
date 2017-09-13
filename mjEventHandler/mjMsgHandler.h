
#ifndef MJ_MSGHANDLER_H__
#define MJ_MSGHANDLER_H__

#include "mjCmdNo.h"
#include <map>
using namespace std;

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
    MsgHandlerMgr(){
        m_Handlers.clear();
    };
    virtual ~MsgHandlerMgr(){
        exit();
    }

public:
    void init() {
        // do something init
    }

    void exit() {
       MsgHandlerIte ite = m_Handlers.begin();
        for ( ite; ite != m_Handlers.end(); ite++ ) {
            if ( ite->second != 0 ){
                delete ite->second;
                ite->second = 0;
            }
        }

        m_Handlers.clear();
    }

    MsgHandler *getHandler(CmdNo msgType) {
        return m_Handlers[msgType];
    }

    void addHandle(CmdNo msgType, MsgHandler *handler) {
        if (handler == 0) {
            return;
        }
        m_Handlers[msgType] = handler;
    }

private:
    typedef map<CmdNo, MsgHandler*> MsgHandlerMap;
    typedef MsgHandlerMap::iterator MsgHandlerIte;

    MsgHandlerMap m_Handlers;
};

#endif  // MJ_MSGHANDLER_H__
