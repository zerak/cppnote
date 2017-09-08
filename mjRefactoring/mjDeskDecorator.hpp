#ifndef CPPNOTE_MJDESKDECORATOR_H
#define CPPNOTE_MJDESKDECORATOR_H

#include "mjDesk.hpp"

class mjDeskDecorator : public mjDesk {
public:
    mjDeskDecorator(mjDesk *desk):m_Desk(desk){}
    virtual ~mjDeskDecorator(){}

    void doCmd(){
        m_Desk->doCmd();
        std::cout<< "mjDeskDecorator doCmd" << std::endl;
    }

protected:
    mjDesk *m_Desk;
};


#endif //CPPNOTE_MJDESKDECORATOR_H
