
#ifndef STATE_START_HPP
#define STATE_START_HPP

#include "AppState.hpp"

class StateStart : public AppState
{
public:
    StateStart();

    DECLARE_APPSTATE_CLASS(StateStart);

    void enter();
    void exit();
    void update(double timeSinceLastFrame);

private:

};

#endif
