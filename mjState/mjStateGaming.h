
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