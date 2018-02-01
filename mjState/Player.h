
#ifndef MJ_PLAYER_H
#define MJ_PLAYER_H

#include <vector>

class MjPlayer
{
public:
    MjPlayer();
    ~MjPlayer();

private:
    char m_dHandList[34];
    vector<char>    m_dChi;
    vector<char>    m_dPeng;
    vector<char>    m_dGang;
};

#endif // MJ_PLAYER_H
