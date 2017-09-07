
#ifndef MJ_DESK_H__
#define MJ_DESK_H__

class MjStateManager;
class MjDesk {
public:
    MjDesk();
    ~MjDesk();

public:
    void start();
    void loop();

public:
    MjStateManager *m_StateMgr;
};

#endif  // MJ_DESK_H__
