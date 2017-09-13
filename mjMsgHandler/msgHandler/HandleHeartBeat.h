
#ifndef HANDLE_HEARTBEAT_HPP
#define HANDLE_HEARTBEAT_HPP

#include "../mjMsgHandler.h"
#include <iostream>

// 出牌
class HandleHeartBeat : public MsgHandler {
public:
    ~HandleHeartBeat(){}

public:
    virtual void processMsg (Packet *pPacket, GamePlayer *pPlayer)
    {
        // process
        std::cout<<"process msg heart beat" << std::endl;
    }
};

#endif //   HANDLE_HEARTBEAT_HPP
