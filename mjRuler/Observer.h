
#ifndef MJ_OBSERVER_H__
#define MJ_OBSERVER_H__

class Observer
{
public:
    virtual onNotify(const Entify& entity, Event event) = 0;
};

#endif // MJ_OBSERVER_H__
