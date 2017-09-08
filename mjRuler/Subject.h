
#ifndef MJ_SUBJECT_H__
#define MJ_SUBJECT_H__

#include <vector>

class Observer;
class Subject
{
public:
    void addObserver(Observer *ob){
        m_pObserver.push_back(ob);
    }

    void removeObserver(Observer *ob){
        auto ite = m_pObserver.begin();
        for(ite; ite != m_pObserver.end(); ite++){
           if(ite == ob){
               m_pObserver.erase(ite);
               break;
           }
        }
    }

    void notify(const Entity &entity, Event event){
        auto ite = m_pObserver.begin();
        for(ite; ite != m_pObserver.end(); ite++){
            ite->onNotify(entity,event);
        }
    }

private:
    std::vector<Observer*> m_pObserver;
};

#endif // MJ_SUBJECT_H__

