#ifndef CPPNOTE_MJDESKDECORATORS3Y_H
#define CPPNOTE_MJDESKDECORATORS3Y_H

#include "mjDeskDecorator.hpp"

// 十三幺
class mjDeskDecoratorS3Y : public mjDeskDecorator {
public:
    mjDeskDecoratorS3Y(mjDesk *desk):mjDeskDecorator(desk){}
    virtual ~mjDeskDecoratorS3Y(){}

    void doCmd(){
        m_Desk->doCmd();

        // s3y do cmd
        // ...
        std::cout<< "mjDeskDecoratorS3Y doCmd" << std::endl;
    }

private:
    int m_S3Y;
};


#endif //CPPNOTE_MJDESKDECORATORS3Y_H
