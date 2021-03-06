
#include <stdio.h>		// file op
#include <cstdlib>		// srand
#include <algorithm>	// random_shuffle
#include <iostream>
#include <string.h>
#include <map>
#include <sys/time.h>
using namespace std;


typedef string Key;
typedef bool Val;
typedef map<Key,Val> tbMap;

class MjMapTbV5 {
private:
#define MAX_HUN_COUNT 8
    typedef int Key;
    typedef bool Val;
    typedef map<Key, Val> tbMap;

public:
    MjMapTbV5() {}
    ~MjMapTbV5() {}

public:
    void init() {
        // gen table data
        genMap();

//        // dump table
//        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
//            dumpFor(m_dFinalFeng[i], getTbName(FengZi, i, false));
//            dumpFor(m_dFinalFengJiang[i], getTbName(FengZi, i, true));
//
//            dumpFor(m_dFinalSanSe[i], getTbName(SanSe, i, false));
//            dumpFor(m_dFinalSanSeJiang[i], getTbName(SanSe, i, true));
//        }
//
//        // print info
//        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
//            cout << "fengKe hun:" << i << " size:" << m_dFinalFeng[i].size() << endl;
//        }
//        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
//            cout << "fengJiangKe hun:" << i << " size:" << m_dFinalFengJiang[i].size() << endl;
//        }
//        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
//            cout << "sanSe hun:" << i << " size:" << m_dFinalSanSe[i].size() << endl;
//        }
//        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
//            cout << "sanseJiang hun:" << i << " size:" << m_dFinalSanSeJiang[i].size() << endl;
//        }
//
//        cleanTmpData();
        return;
    }

    // handList: 当前手牌
    // len: handList长度
    /*
        手牌格式
        int cards[35] = {			  // 仅处理前35个字节
        0,                            // 混个数
        1, 0, 0, 0, 0, 0, 1,          // 风字牌1-7
        0, 0, 0, 0, 0, 0, 0, 0, 0,    // 万8-16
        0, 0, 0, 0, 0, 0, 3, 0, 0,    // 条17-25
        0, 0, 0, 0, 0, 0, 0, 0, 0,    // 筒26-34
        0, 0, 0, 0, 0, 0, 0, 0        // 花35-42
        };
    */
    bool checkHu(char* handList, int len) {
        int hunNum = handList[0];
        if (hunNum > MAX_HUN_COUNT) {
            hunNum = MAX_HUN_COUNT;
        }
        return split(handList + 1, hunNum);
    }

private:
    enum MjType {
        SanSe = 0,
        FengZi = 1
    };
    enum MjColorType {
        coFengZi = 0,
        coWan = 1,
        coTiao = 2,
        coTong = 3,
        coMax = 4
    };

    bool check(int hunNeed, int jiangNum, int hunNum, int& hunSum) {
        if (hunNeed < 0)
            return false;

        hunSum += hunNeed;
        if (hunSum > hunNum)
            return false;

        if (jiangNum == 0)
            return true;

        return hunSum + (jiangNum - 1) <= hunNum;
    }
    bool split(char* const cards, int hunNum) {
        int jiangNum = 0;
        int hunSum = 0;
        int hunNeed = 0;

        hunNeed = _split(cards, hunNum, 0, 6, false, jiangNum);
        if (!check(hunNeed, jiangNum, hunNum, hunSum))
            return false;

        hunNeed = _split(cards, hunNum - hunSum, 7, 15, true, jiangNum);
        if (!check(hunNeed, jiangNum, hunNum, hunSum))
            return false;

        hunNeed = _split(cards, hunNum - hunSum, 16, 24, true, jiangNum);
        if (!check(hunNeed, jiangNum, hunNum, hunSum))
            return false;

        hunNeed = _split(cards, hunNum - hunSum, 25, 33, true, jiangNum);
        if (!check(hunNeed, jiangNum, hunNum, hunSum))
            return false;

        if (jiangNum == 0) {
            return hunSum + 2 <= hunNum;
        }

        return true;
    }
    int _split(char* const cards, int hunNum, int min, int max, bool isSanSe, int& jiangNum) {
        int key = 0;
        int num = 0;

        MjType tpe = isSanSe ? SanSe : FengZi;

        for(int i = min; i <= max; ++i) {
            key = key*10 + cards[i];
            num = num + cards[i];
        }

        if (num == 0)
            return 0;

        for (int i = 0; i <= hunNum; ++i) {
            int yu = (num + i) % 3;
            if (yu == 1)
                continue;
            bool hasJiang = (yu == 2);
            if (isHu(key, i, hasJiang, tpe)) {
                if (hasJiang)
                    jiangNum++;
                return i;
            }
        }

        return -1;
    }
    bool isHu(int key, int hunNum, bool hasJiang, MjType tpe) {
        tbMap &d = getMap(tpe, hunNum, hasJiang);
        if (d.find(key) == d.end()) {
            return false;
        }
        return true;
    }

    Val getVal() {
        return Val(true);
    }

    tbMap& getMap(MjType tpe, int hunNum, bool hasJiang) {
        if (hasJiang) {
            if (tpe == SanSe) {
                return m_dFinalSanSeJiang[hunNum];
            } else {
                return m_dFinalFengJiang[hunNum];
            }
        } else {
            if (tpe == SanSe) {
                return m_dFinalSanSe[hunNum];
            } else {
                return m_dFinalFeng[hunNum];
            }
        }
    }
    tbMap& getTmpMap(MjType tpe, int hunNum, bool hasJiang) {
        if (hasJiang) {
            if (tpe == SanSe) {
                return m_dSanSeJiangTmp[hunNum];
            } else {
                return m_dFengJiangTmp[hunNum];
            }
        } else {
            if (tpe == SanSe) {
                return m_dSanSeTmp[hunNum];
            } else {
                return m_dFengTmp[hunNum];
            }
        }
    }

    void dumpFor(tbMap &tbl, string name) {
        FILE *fp = fopen(name.c_str(), "wb+");
        if(!fp) {
            cout << "can't open file:" << name.c_str() << " for write." << endl;
            return;
        }

        for (tbMap::iterator ite = tbl.begin(); ite != tbl.end(); ite++) {
            Key key = ite->first;
            fprintf(fp, "%d\n", key);
        }
        fclose(fp);
    }

    bool add(tbMap &tm, Key &key, Val &val) {
        tm.insert(make_pair(key, val));
        return true;
    }
    bool check_add(int *cards, int hunNum, bool hasJiang, MjType tpe) {
        int key = 0;
        for (int i = 0; i < 9; i++) {
            key = key * 10 + cards[i];
        }

        if (key == 0) {
            return false;
        }

        tbMap &tmpMap = getTmpMap(tpe, hunNum, hasJiang);
        if(tmpMap.find(key) != tmpMap.end()){
            return false;
        }
        tmpMap[key] = true;

        for (int i = 0; i < 9; i++) {
            if (cards[i] > 4) {
                return true;
            }
        }

        Val val = getVal();
        return add(getMap(tpe, hunNum, hasJiang), key, val);
    }
    void parse_table_sub(int *cards, int hunNum, bool hasJiang, MjType tpe) {
        for (int i = 0; i < 9; i++) {
            if (cards[i] == 0) {
                continue;
            }

            cards[i]--;

            if (!check_add(cards, hunNum, hasJiang, tpe)) {
                cards[i]++;
                continue;
            }

            if (hunNum < MAX_HUN_COUNT) {
                parse_table_sub(cards, hunNum + 1, hasJiang, tpe);
            }
            cards[i]++;
        }
    }
    void parse_table(int *cards, bool hasJiang, MjType tpe) {
        if (!check_add(cards, 0, hasJiang, tpe)) {
            return;
        }
        parse_table_sub(cards, 1, hasJiang,tpe);
    }

    void genSanSeSub(int *cards, int level, bool hasJiang) {
        for(int i = 0; i < 16; ++i) {
            if(i <= 8) {
                if(cards[i] > 3)
                    continue;
                cards[i] += 3;
            } else {
                int index = i - 9;
                if(cards[index] > 5 || cards[index+1] > 5 || cards[index+2] > 5)
                    continue;
                cards[index] += 1;
                cards[index+1] += 1;
                cards[index+2] += 1;
            }

            parse_table(cards, hasJiang, SanSe);
            if(level < 4) {
                genSanSeSub(cards, level + 1, hasJiang);
            }

            if(i <= 8) {
                cards[i] -= 3;
            } else {
                int index = i - 9;
                cards[index] -= 1;
                cards[index + 1] -= 1;
                cards[index + 2] -= 1;
            }
        }
    }
    void genSanSeKeShun() {
        int cards[34] = {0};
        genSanSeSub(cards, 1, false);
    }
    void genSanSeJiangKeShun() {
        int cards[34] = {0};
        for(int i = 0; i < 9; ++i) {
            cards[i] = 2;
            parse_table(cards, true, SanSe);
            genSanSeSub(cards, 1, true);
            cards[i] = 0;
        }
    }
    void genSanSeMap() {
        cout << "begin gen sanse ke/shun..." << endl;
        genSanSeKeShun();
        cout << "sanse ke/shun gen end" << endl;

        cout << "begin gen sanse jiang + ke/shun..." << endl;
        genSanSeJiangKeShun();
        cout << "sanse jiang + ke/shun gen end" << endl;
    }

    void genFengSub(int *cards, int level, bool hasJiang) {
        for(int i=0; i < 7; ++i) {
            if(cards[i] > 3)
                continue;

            cards[i] += 3;

            parse_table(cards, hasJiang, FengZi);
            if(level<4) {
                genFengSub(cards, level + 1, hasJiang);
            }
            cards[i] -= 3;
        }
    }
    void genFengKe() {
        int card[34] = {0};
        genFengSub(card, 1, false);
    }
    void genFengJiangKe() {
        int cards[34] = {0};
        for(int i=0; i<7; ++i) {
            cards[i] = 2;
            parse_table(cards, true, FengZi);
            genFengSub(cards, 1, true);
            cards[i] = 0;
        }
    }
    void genFengMap() {
        cout << "begin gen feng ke..." << endl;
        genFengKe();
        cout << "feng ke gen end" << endl;

        cout << "begin gen fengJiang ke..." << endl;
        genFengJiangKe();
        cout << "fengJiang ke gen end" << endl;
    }

    void genMap() {
        double timeUse = 0;
        struct timeval start, end;
        gettimeofday(&start, NULL);
        genSanSeMap();	// san se map
        gettimeofday(&end, NULL);
        timeUse = end.tv_sec - start.tv_sec;
        cout << "gen sanse map cost:" << timeUse << "s" << endl;

        gettimeofday(&start, NULL);
        genFengMap();	// feng map
        gettimeofday(&end, NULL);
        timeUse = end.tv_sec - start.tv_sec;
        cout << "gen feng map cost:" << timeUse << "s" << endl;
    }

    string itos(int num) {
        string str;
        char buf[] = {0, 0, 0, 0};
        sprintf(buf, "%d", num);
        str = buf;
        return str;
    }
    string getTbName(MjType tpe, int hunNum, bool hasJiang) {
        string name = "";
        if (tpe == SanSe) {
            if(hasJiang) {
                name = "tbl/table_sanse_jiang_" + itos(hunNum);
            } else {
                name = "tbl/table_sanse_" + itos(hunNum);
            }
        } else {
            if (hasJiang) {
                name = "tbl/table_feng_jiang_" + itos(hunNum);
            } else {
                name = "tbl/table_feng_" + itos(hunNum);
            }
        }
        name += ".tbl";
        return name;
    }
    bool loadTb(MjType tpe, int hunNum, bool hasJiang) {
        string name = getTbName(tpe, hunNum, hasJiang);
        FILE *fp = fopen(name.c_str(), "rb");
        if (fp == NULL) {
            cout << "can't load table:" << name.c_str() << endl;
            return false;
        }

        int num = 0;
        int key = 0;
        while (fscanf(fp, "%d\n", &key) != EOF) {
            Val val = getVal();
            add(getMap(tpe, hunNum, hasJiang), key, val);
            num++;
        }
        cout << "load table:" << name.c_str() << " " << num << "times, size:" << getMap(tpe, hunNum, hasJiang).size() << endl;
        fclose(fp);
        return true;
    }
    void load() {
        // load table
        for (int i = 0; i < MAX_HUN_COUNT + 1; ++i) {
            loadTb(FengZi, i, true);
            loadTb(FengZi, i, false);

            loadTb(SanSe, i, true);
            loadTb(SanSe, i, false);
        }
    }

    void cleanTmpData() {
        for (int i = 0; i < MAX_HUN_COUNT + 1; ++i) {
            m_dSanSeTmp[i].clear();
            m_dSanSeJiangTmp[i].clear();
            m_dFengTmp[i].clear();
            m_dFengJiangTmp[i].clear();
        }
    }

private:
    tbMap m_dFinalSanSe[MAX_HUN_COUNT+1];		// 最终存放带混三色数据
    tbMap m_dFinalSanSeJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混三色数据

    tbMap m_dFinalFeng[MAX_HUN_COUNT+1];		// 最终存放带混风字数据
    tbMap m_dFinalFengJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混风字数据

    // 临时Map，用于临时生成检测Key
    // 生成数据后清理
    tbMap m_dSanSeTmp[MAX_HUN_COUNT+1];
    tbMap m_dSanSeJiangTmp[MAX_HUN_COUNT+1];
    tbMap m_dFengTmp[MAX_HUN_COUNT+1];
    tbMap m_dFengJiangTmp[MAX_HUN_COUNT+1];
};

Val getVal() {
    return true;
}

bool add(tbMap &tm, Key &key, Val &val) {
    tbMap::iterator ite = tm.find(key);
    if (ite != tm.end()) {
        return false;
    }
    tm.insert(make_pair(key,val));
    return true;
}

bool loadOf(tbMap &d, string path) {
    char name[256];
    sprintf(name, "tbl/table_%s.tbl", path.c_str());
    FILE *fp = fopen(name, "rb");
    if (fp == NULL) {
        cout << "can't load table:" << name << endl;
        return false;
    }

    char str[43];
    memset(str, 0, sizeof(str));
    int num = 0;
    while (fscanf(fp, "%s\n", str) != EOF) {
        num++;
        Key key(str);
        Val val = getVal();
        add(d, key, val);
//        if (num == 1000000) {
//			break;
//		}
    }
    cout << "load table:" << name << " set_size:" << num << endl;
    fclose(fp);
    return true;
}

void print_cards(char* cards) {
    printf("%d, ", cards[0]);
    for (int i = 1; i <= 7; ++i) {
        printf("%d,", cards[i]);
    }
    printf("  ");

    for (int i = 8; i <= 16; ++i) {
        printf("%d,", cards[i]);
    }
    printf("  ");

    for (int i = 17; i <= 25; ++i) {
        printf("%d,", cards[i]);
    }
    printf("  ");

    for (int i = 26; i <= 34; ++i) {
        printf("%d,", cards[i]);
    }
//    printf("  ");
//
//    for (int i = 35; i <= 42; ++i) {
//        printf("%d,", cards[i]);
//    }

    printf("\n");
}

tbMap tb;
int main(int argc, char **argv) {
    MjMapTbV5 v5;
    v5.init();
    cout << "tran all" << endl;

    cout << "begin load" << endl;
    loadOf(tb, "total");
    cout << "end load len:" << tb.size() << endl;

    int num = 0;
    char cards[36] = { 0 };
    for (tbMap::iterator ite = tb.begin(); ite != tb.end(); ++ite) {
//			title{
//					0,					// 混
//					0,0,0,0,0,0,0,		// 风字牌1-7
//					0,0,0,0,0,0,0,0,0,	// 万8-16
//					0,0,0,0,0,0,0,0,0,	// 条17-25
//					0,0,0,0,0,0,0,0,0	// 筒26-34
//			};
        string key = ite->first;
//		string key = "12000002300000003400000004500000005";
        for (int i = 0; i < 35; ++i) {
            if (i <= 0) {
                cards[0] = key[i] - '0';
            } else if (i > 0) {
                cards[i] = key[i] - '0';
            }
        }

        // check
        if (v5.checkHu(cards, 34)) {
            ++num;
        } else {
            cout << "key:" << key << endl;
            print_cards(cards);
        }
        memset(cards, 0, sizeof(cards));
    }

    cout << "hu:" << num << " not check count:" << tb.size() - num << endl;
    return 0;
}
