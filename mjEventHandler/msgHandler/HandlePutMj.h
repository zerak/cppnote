
#ifndef HANDLE_PUTMJ_HPP
#define HANDLE_PUTMJ_HPP

#include "../mjMsgHandler.h"
#include <iostream>

// 出牌
class HandlePutMj : public MsgHandler {
public:
    ~HandlePutMj(){}

public:
    virtual void processMsg (Packet *pPacket, GamePlayer *pPlayer)
    {
        // process
        std::cout<<"process msg put mj" << std::endl;
    }
};

#endif //   HANDLE_PUTMJ_HPP
