
#include "mjMsgHandler.h"
#include "msgHandler/HandleHeartBeat.h"
#include "msgHandler/HandlePutMj.h"
#include "mjRoomManager.h"
#include <iostream>
#include <limits>

MjRoomManger::MjRoomManger() {
    m_pMsgMgr = new MsgHandlerMgr;
}

MjRoomManger::~MjRoomManger() {
    if ( m_pMsgMgr != 0 ) {
        delete m_pMsgMgr;
        m_pMsgMgr = 0;
    }
}

void MjRoomManger::regMsg() {
    // register msg
    m_pMsgMgr->addHandle(CmdNo_HeartBeat,new HandleHeartBeat);
    m_pMsgMgr->addHandle(CmdNo_PutMj,new HandlePutMj);
}

void MjRoomManger::processMsg(int cmdNo) {
    MsgHandler* handle = m_pMsgMgr->getHandler(CmdNo(cmdNo));
    // just for test
    if ( handle ) {
        handle->processMsg(0,0);
    } else {
        std::cout << cmdNo <<" cmd err" << std::endl;
    }
}

void MjRoomManger::loop() {
    // get room
    // process msg
}

// entry
int main() {
    MjRoomManger mgr;
    mgr.regMsg();
    mgr.loop();

    int cmdNo = 0;
    while (cmdNo != -1) {
        std::cout<< "(-1 quit) input msg cmd->";
        if (!(std::cin>>cmdNo)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout<< "enter number only exit"<< std::endl;
            continue;
        }
        std::cout<< "cmd no is: "<< cmdNo << std::endl;
        mgr.processMsg(cmdNo);
    }

    return  0;
}
