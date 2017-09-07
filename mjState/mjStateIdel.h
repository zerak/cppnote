
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