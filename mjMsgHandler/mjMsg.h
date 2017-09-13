
#ifndef MJ_MSG_H__
#define MJ_MSG_H__

namespace Msg
{
    // 出牌
    class PutMj {
       int seatId;
       int mjCode;
    };

    // 拿牌
    class GetMj {
        int seatId;
        int mjCode;
    };

    // ...
}

#endif  // MJ_MSG_H__
