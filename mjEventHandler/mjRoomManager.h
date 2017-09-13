
#ifndef MJ_ROOMMANAGER_H__
#define MJ_ROOMMANAGER_H__

class MsgHandlerMgr;
class MjRoomManger {
public:
    MjRoomManger();
    ~MjRoomManger();

public:
    void regMsg();
    void processMsg(int cmdNo);
    void loop();

private:
    MsgHandlerMgr* m_pMsgMgr;
};

#endif  // MJ_ROOMMANAGER_H__
