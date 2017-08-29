#ifndef CPPNOTE_MJDESKDECORATORQ2S_H
#define CPPNOTE_MJDESKDECORATORQ2S_H

#include "mjDeskDecorator.hpp"

// 清一色
class mjDeskDecoratorQ1S : public mjDeskDecorator {
public:
    mjDeskDecoratorQ1S(mjDesk *desk):mjDeskDecorator(desk){}
    ~mjDeskDecoratorQ1S(){}

    void doCmd(){
        m_Desk->doCmd();

        // q1s do cmd
        // ...
        std::cout<< "mjDeskDecoratorQ1S doCmd" << std::endl;
    }

private:
    int m_Q1S;
};

#endif //CPPNOTE_MJDESKDECORATORQ2S_H
