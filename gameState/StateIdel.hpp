
#ifndef STATE_IDEL_HPP
#define STATE_IDEL_HPP

#include "AppState.hpp"

class StateIdel : public AppState
{
public:
    StateIdel();

    DECLARE_APPSTATE_CLASS(StateIdel);

    void enter();
    void exit();
    void update(double timeSinceLastFrame);

private:
    bool m_bQuit;
};

#endif
