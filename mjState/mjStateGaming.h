
#ifndef MJ_STATEGAMING_H__
#define MJ_STATEGAMING_H__

#include "mjState.h"

class MjStateGaming : public MjState {
public:
    MjStateGaming();

    DECLARE_STATE_CLASS(MjStateGaming)

public:
    void enter();
    void exit();

    void update();
};

#endif //   MJ_STATEGAMING_H__
