
#ifndef HANDLE_PUTMJ_HPP
#define HANDLE_PUTMJ_HPP

#include "../mjEventHandler.h"

class HandlePutMj : public EventHandler {
public:
    ~HandlePutMj(){}

public:
    virtual void processEvent(Packet *pPacket, GamePlayer *pPlayer)
    {
        // process
    }
};

#endif //   HANDLE_PUTMJ_HPP
