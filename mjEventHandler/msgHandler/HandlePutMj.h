
#ifndef HANDLE_PUTMJ_HPP
#define HANDLE_PUTMJ_HPP

#include "../mjMsgHandler.h"

// 出牌
class HandlePutMj : public MsgHandler {
public:
    ~HandlePutMj(){}

public:
    virtual void processMsg (Packet *pPacket, GamePlayer *pPlayer)
    {
        // process
    }
};

#endif //   HANDLE_PUTMJ_HPP
