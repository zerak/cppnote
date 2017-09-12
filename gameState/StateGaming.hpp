
#ifndef STATE_IDEL_HPP
#define STATE_IDEL_HPP

#include "AppState.hpp"

class StateGaming : public AppState
{
public:
    StateGaming();

    DECLARE_APPSTATE_CLASS(StateGaming);

    void enter();
    void exit();
    void update(double timeSinceLastFrame);

private:
    bool m_bQuit;
};

#endif
