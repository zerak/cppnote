
#include <stdlib.h> // itoa
#include <stdio.h>
#include <unistd.h> // sleep
#include <cstdlib>	// srand
#include <iostream>
#include <string.h>
#include <algorithm>
#include <map>
#include <vector>
#include <sys/time.h>
#include <math.h>   // pow
using namespace std;

typedef int Key;
typedef int Val;
typedef map<Key,Val> tbMap;
typedef vector<Key> tbVec;

#define MAX_MJ_CODEARRAY 34
/*
title {
    0,0,0,0,0,0,0,		// 风字牌1-7
    0,0,0,0,0,0,0,0,0,	// 万8-16
    0,0,0,0,0,0,0,0,0,	// 条17-25
    0,0,0,0,0,0,0,0,0,	// 筒26-34
};
*/

class MjMapTbV2 {
public:
    MjMapTbV2(){}
    ~MjMapTbV2(){}

public:
    enum MjType {
        SanSe = 0,
        FengZi = 1
    };

    Val getVal() {
        return Val(0);
    }

    tbMap& getMap(MjType tpe) {
        if (tpe == SanSe) {
            return m_dSanSe;
        }

        return m_dFeng;
    }

    void add(tbMap &tm, Key &key, Val &val) {
        tbMap::iterator ite = tm.find(key);
        if (ite != tm.end()) {
            return;
        }
        tm.insert(make_pair(key,val));
    }

    // todo
    string getTbName(MjType tpe) {
        if (tpe == SanSe) {
            return "sanse";
        }

        return "feng";
    }

    bool loadTb(MjType tpe) {
        char name[256];
        sprintf(name, "tbl/table_%s.tbl", getTbName(tpe).c_str());
        FILE *fp = fopen(name, "rb");
        if (fp == NULL) {
            cout << "can't load table:" << name << endl;
            return false;
        }

        int num = 0;
        int key = 0;
        while (fscanf(fp, "%d\n", &key) != EOF) {
            Val val = getVal();
            add(getMap(tpe), key, val);
            num++;
        }
        cout << "load table:" << name << " " << num << "times, size:" << getMap(tpe).size() << endl;
        fclose(fp);
        return true;
    }

    void load() {
        if (!loadTb(SanSe)) {
            genSanSeKe();       // 生成三色牌Ke
            genSanSeShun();     // 生成三色牌ShunZi
            genFengKe();        // 生成风字牌Ke

            genSanSeMap();
            genFengMap();
            return;
        }

        cout << "sanseKe:" << m_dSanSeKe.size() << endl;
        cout << "sanseShun:" << m_dSanSeShun.size() << endl;
        cout << "fengKe:" << m_dFengKe.size() << endl;

        loadTb(SanSe);
        loadTb(FengZi);
    }

    bool valid(Key key) {
        char buf[32];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d", key);
        //itoa(key, buf, 10);
        for (int i = 0; i < 32; ++i) {
            if (buf[i]-'0' > 4) {
                return false;
            }
        }
        return true;
    }

    Key KeyOpAdd(Key k1, Key k2) {
        return k1 + k2;
    }

    // d data set
    // tbTmp save tmp data
    void chooseFrom(tbVec &d, tbMap &tbTmp, int choose, int from) {
        if (choose <= 0 || choose > 4)
            return;

//        tbTmp.clear();

        int values[from];
        for (int i = 0; i < from; ++i) {
            values[i] = i+1;
        }

        int elements[sizeof(values)/ sizeof(values[0])];
        memset(elements, 0, sizeof(elements));
        for (int i = 0; i < choose; ++i) {
            elements[i] = 1;
        }
        const size_t N = sizeof(elements)/sizeof(elements[0]);
        std::vector<int> selectors(elements, elements + N);

        int count = 0;
        Key chooseKey[choose];
        do {
            ++count;
            int ch = 0;
            for (size_t i = 0; i < selectors.size(); ++i) {
                if (selectors[i]>0) {
                    //cout << values[i] << ",i:" << i << " " << d[values[i]-1];
                    Key key = d[values[i]-1];   // 获取第几个值
                    chooseKey[ch] = key;
                    ch++;
                }
            }
            //cout << endl;

            Key tmp = chooseKey[0];
            for (int j = 1; j < choose; j++) {
                //cout << "opAdd:" << tmp << " and " << chooseKey[j] << endl;
                tmp = KeyOpAdd(tmp, chooseKey[j]);
                //cout << "result:" << tmp << endl;
            }

            if (valid(tmp)) {
                tbTmp.insert(make_pair(tmp, getVal()));
            }

            //cout << "bkey:" << tmp << endl;

            /*
            std::cout << std::endl;
            if (count==10){
                break;
            }
            sleep(0.1);
             */
        } while (prev_permutation(selectors.begin(), selectors.end()));

        //cout << "total count:" << count << " tmMapSize:" << tbTmp.size() << endl;
    }

    Key getKey(int *list, int size) {
        Key key = 0;
        for (int i = 0; i < size; ++i) {
            key += list[i] * pow(10,size-(i+1));
        }
        return key;
    }

    tbVec& genSanSeKe() {
        int title [9];
        memset(title, 0, sizeof(title));
        for (int i = 0; i < 9; ++i) {
            title[i] = 3;
            Key key = getKey(title, 9);
            m_dSanSeKe.push_back(key);
            //cout << "sanseKe:" << key << endl;
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dSanSeKe,"sanseKe");
        return m_dSanSeKe;
    }
    tbVec& genSanSeShun() {
        int title [9];
        memset(title, 0, sizeof(title));
        for (int i = 0; i < 7; i++) {
            title[i] = 1;
            title[i+1] = 1;
            title[i+2] = 1;
            Key key = getKey(title, 9);
            m_dSanSeShun.push_back(key);
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dSanSeShun,"sanseShun");
        return m_dSanSeShun;
    }
    tbVec& genFengKe() {
        int title [7];
        memset(title, 0, sizeof(title));
        for (int i = 0; i < 7; ++i) {
            title[i] = 3;
            Key key = getKey(title, 7);
            m_dFengKe.push_back(key);
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dFengKe,"fengKe");
        return m_dFengKe;
    }
    void dumpTest(tbVec &d, string tpe){
        char name[256];
        sprintf(name, "tbl/table_%s.tbl", tpe.c_str());
        FILE *fp = fopen(name, "wb+");

        for (tbVec::iterator ite = d.begin(); ite != d.end(); ite++) {
            Key key = *ite;
            fprintf(fp, "%d\n", key);
        }
        fclose(fp);
    }
    void dumpTest(tbMap &d, string tpe){
        char name[256];
        sprintf(name, "tbl/table_%s.tbl", tpe.c_str());
        FILE *fp = fopen(name, "wb+");

        tbMap tbl = d;

        for (tbMap::iterator ite = tbl.begin(); ite != tbl.end(); ite++) {
            Key key = ite->first;
            fprintf(fp, "%d\n", key);
        }
        fclose(fp);
    }

    void caseKeShun(tbVec &vecKe, tbVec &vecShun, int countKe, int countShun) {
        tbMap tmpData;
        chooseFrom(vecKe, tmpData, countKe, vecKe.size());
        //cout << "chooseKe size:" << vecKe.size() << endl;
        for (tbMap::iterator iteKe = tmpData.begin(); iteKe != tmpData.end(); iteKe++) {
            Key tmp = iteKe->first;
            if (valid(tmp)) {
                //cout << iteKezi->first << " keKey" << endl;
                //cout << tmp << " ke" << endl;
                tbMap tmpDataShun;
                chooseFrom(vecShun, tmpDataShun, countShun, vecShun.size());
                if (countShun <= 0) {
                    m_dSanSe.insert(make_pair(tmp, getVal()));
                } else {
                    //cout << "chooseShunzi size:" << m_dChooseShunzi.size() << endl;
                    for (tbMap::iterator iteShun = tmpDataShun.begin(); iteShun != tmpDataShun.end(); iteShun++) {
                        Key tmp2 = KeyOpAdd(tmp, iteShun->first);
                        if(valid(tmp2)) {
                            //cout << iteShunzi->first << " shunKey" << endl;
                            m_dSanSe.insert(make_pair(tmp2, getVal()));
                            //cout << tmp2 << " " << countKe << "ke " << countShun << "shun key" << endl;
                            //break;
                        }
                    }
                }
                //break;
            }
        }
    }
    void genSanSe1() {
        // ke1 shun0
        // ke0 shun1
        chooseFrom(m_dSanSeKe, m_dSanSe, 1, m_dSanSeKe.size());    // ke c(9/1)
        chooseFrom(m_dSanSeShun, m_dSanSe, 1, m_dSanSeShun.size());    // shun c(7/1)
    }
    void genSanSe2() {
        // ke2 shun0
        // ke1 shun1
        // ke0 shun2
        // 2-0
        chooseFrom(m_dSanSeKe, m_dSanSe, 2, m_dSanSeKe.size());    // ke c(9/2)

        // 1-1
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 1);

        // 0-2
        chooseFrom(m_dSanSeShun, m_dSanSe, 2, m_dSanSeShun.size());     // shun c(7/2)
    }
    void genSanSe3() {
        // ke3 shun0
        // ke2 shun1
        // ke1 shun2
        // ke0 shun3
        // 3-0
        chooseFrom(m_dSanSeKe, m_dSanSe, 3, m_dSanSeKe.size());

        // 2-1
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 2, 1);

        // 1-2
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 2);

        // 0-3
        chooseFrom(m_dSanSeShun, m_dSanSe, 3, m_dSanSeShun.size());
    }
    void genSanSe4() {
        // ke4 shun0
        // ke3 shun1
        // ke2 shun2
        // ke1 shun3
        // ke0 shun4
        // 4-0
        chooseFrom(m_dSanSeKe, m_dSanSe, 4, m_dSanSeKe.size());

        // 3-1
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 3, 1);

        // 2-2
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 2, 2);

        // 1-3
        caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 3);

        // 0-4
        chooseFrom(m_dSanSeShun, m_dSanSe, 4, m_dSanSeShun.size());
    }
    void genSanSeMap() {
        genSanSe1();
        genSanSe2();
        genSanSe3();
        genSanSe4();
        dumpTest(m_dSanSe,"sanse");
        cout << "sanseKeShun size:" << m_dSanSe.size() << endl;
    }

    void genFengMap() {
        chooseFrom(m_dFengKe, m_dFeng, 1, m_dFengKe.size());
        chooseFrom(m_dFengKe, m_dFeng, 2, m_dFengKe.size());
        chooseFrom(m_dFengKe, m_dFeng, 3, m_dFengKe.size());
        chooseFrom(m_dFengKe, m_dFeng, 4, m_dFengKe.size());
        dumpTest(m_dFeng,"feng");
        cout << "fengKe size:" << m_dFeng.size() << endl;
    }

    void genMap() {
        genSanSeMap();
        genFengMap();
    }

    bool check(Key key, MjType tpe) {
        tbMap &d = getMap(tpe);
        tbMap::iterator ite = d.find(key);
        if (ite == d.end()) {
            return false;
        }
        return true;
    }
private:
    int m_dTotal; 		// 总胡牌数
    tbMap m_dSanSe;     // 最终存放生成三色数据
    tbMap m_dFeng;      // 最终存放生成风字数据

    // tmpData
    tbVec m_dSanSeKe;       // 用于生成三色牌Ke c(n/m)
    tbVec m_dSanSeShun;     // 用于生成三色牌Shun c(n/m)
    tbVec m_dFengKe;        // 用于生成风牌Ke c(n/m)
};

void benchmark(MjMapTbV2 &tb) {
    int MAX_COUNT =	100 * 10000;
    int GUI_NUM = 1;
    int source[MAX_COUNT * 9 * MAX_MJ_CODEARRAY];
    int allCards[144];
    int code = 0;
    for (int i = 0; i < 34; i++) {
        allCards[i*4] = code+1;
        allCards[i*4+1] = code+1;
        allCards[i*4+2] = code+1;
        allCards[i*4+3] = code+1;
        code++;
    }
    allCards[135] = 35;
    allCards[136] = 36;
    allCards[137] = 37;
    allCards[138] = 38;
    allCards[139] = 39;
    allCards[140] = 40;
    allCards[141] = 41;
    allCards[142] = 42;
    allCards[143] = 43;

    int total = 0;
    srand(1);
    cout << "shuffle begin" << endl;

    double timeUse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start,NULL);
    for (int n = 0; n < MAX_COUNT; ++n) {
        random_shuffle(allCards, allCards + 136);
        for (int i = 0; i < 9; ++i) { // 136/14 -> 9
            int* cards = &source[total++ * MAX_MJ_CODEARRAY];
            memset(cards, 0, MAX_MJ_CODEARRAY);
            for (int j = i * 14; j < i * 14 + 14; j++)
                ++cards[allCards[j]];
        }
    }
    gettimeofday(&end, NULL);
    timeUse = end.tv_sec-start.tv_sec;
    cout << "shuffle end cost:" << timeUse << "s" << endl;

    // hu check
    gettimeofday(&start, NULL);
    int hu = 0;
    for (int n = 0; n < total; ++n) {
        //Key key = tb.getKey(source+n * 43, 43);
        //cout << "check key:" << key << endl;
        //hu += tb.check(key);
    }
    gettimeofday(&end,NULL);
    timeUse = (end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec)/1000;
    cout << "check " << MAX_COUNT << " hu:" << hu << " cost:" << timeUse << "ms" << endl;
}

void loadWithoutGen() {
    double timeUse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start,NULL);

    MjMapTbV2 mt;
    cout << "begin load map..." << endl;
    //mt.loadOf(mt.m_dTmp, "total");

    gettimeofday(&end,NULL);
    timeUse = end.tv_sec-start.tv_sec;
    cout << "load map use:" << timeUse << "s" << endl;

    cout << "begin benchmark..." << endl;
    benchmark(mt);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "USAGE:\n"
                "1,gen map to file.\n"
                "2,benchmark." << endl;
        return 0;
    }

    int tpe = atoi(argv[1]);
    if (tpe==1) {
        // gen map
        MjMapTbV2 mt;
        mt.load();
        int tmp;
        cin >> tmp;
    } else if (tpe == 2) {
        // benchmark
        loadWithoutGen();
    } else {
        cout << "arg err check usage." << endl;
        cout << "1,gen map to file.\n"
                "2,benchmark." << endl;
    }

    return 0;
}
