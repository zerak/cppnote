
#ifndef MJ_STATEIDEL_H__
#define MJ_STATEIDEL_H__

#include "mjState.h"

class MjStateIdel : public MjState {
public:
    MjStateIdel();

    DECLARE_STATE_CLASS(MjStateIdel)

public:
    void enter();
    void exit();

    void update();
};

#endif //   MJ_STATEIDEL_H__
