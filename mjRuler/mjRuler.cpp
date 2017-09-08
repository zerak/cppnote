
#include <iostream>
#include "mjRulerBSB.h"

class MjDesk : public Observer
{
public:
    virtual void onNotify(const Entity& entity, Event event){
        std::cout<<"recv notify"<< std::endl;
    }

private:
};

int main(){
    MjDesk desk;
    MjRulerBSB bsb; // 边三饼
    bsb.addObserver(desk);

}
