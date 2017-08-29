#include "mjDesk.hpp"
#include "mjDeskDecorator.hpp"
#include "mjDeskDecoratorS3Y.hpp"
#include "mjDeskDecoratorQ1S.hpp"

using namespace std;

int main() {
//    mjDesk *desk = new mjDesk;
//    mjDesk *rulerS3y = new mjDeskDecoratorS3Y(desk);
//    mjDesk *rulerQ1S = new mjDeskDecoratorQ1S(desk);
//    mjDesk *rulerQ1S_S3Y = new(mjDeskDecoratorS3Y(rulerQ1S));
//
//    rulerS3y->doCmd();
//    rulerQ1S->doCmd();
//    rulerQ1S_S3Y->doCmd();

//    auto shanDong_jiNan( new mjDeskDecoratorS3Y(desk) );
//    shanDong_jiNan->doCmd();

    mjDesk desk;
    mjDeskDecoratorS3Y rulerS3y(&desk);
//    mjDeskDecoratorQ1S rulerQ1S(&desk);
    mjDeskDecoratorQ1S rulerQ1S(&rulerS3y);
//    rulerS3y.doCmd();
    rulerQ1S.doCmd();

    return 0;
}
