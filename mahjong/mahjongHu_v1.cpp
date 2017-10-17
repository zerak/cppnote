
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep
#include <cstdlib>	// srand
#include <iostream>
#include <string.h>
#include <algorithm>
#include <map>
#include <vector>
#include <sys/time.h>
using namespace std;

typedef string Key;
typedef string Val;
typedef map<Key,Val> tbMap;

#define MAX_HUN_COUNT 8
#define MAX_MJ_CODEARRAY 43

class MjMapTb {
public:
    MjMapTb(){}
    ~MjMapTb(){}

    void cleanMap() {
        for (int i = 0; i <= MAX_HUN_COUNT; i++)
        {
            m_dMapTb[i].clear();
        }
        m_dJiang.clear();
        m_dShunzi.clear();
        m_dKezi.clear();
    }

    Key getKey(int *mjList, int len) {
        char tmp[len+1];
        for (int i = 0; i < len; i++) {
            tmp[i] = '0' + mjList[i];
        }
        tmp[len] = '\0';
        Key key(tmp);
        return key;
    }

    Val getVal() {
        return "";
    }

    tbMap& getMap(int hunCount) {
        return m_dMapTb[hunCount];
    }

    bool add(tbMap &tm, Key &key, Val &val) {
        tbMap::iterator ite = tm.find(key);
        if (ite != tm.end()) {
            return false;
        }
        tm.insert(make_pair(key,val));
        //cout << "insert map key:" << key << endl;
        return true;
    }

    void load() {
        bool done = true;
        char name[256];
        for (int i = 0; i <= MAX_HUN_COUNT; i++) {
            sprintf(name, "tbl/table_%d.tbl", i);
            FILE *fp = fopen(name, "rb");
            if (fp == NULL)
            {
                cout << "can't load table:" << name << endl;
                done = false;
                break;
            }

            char str[MAX_MJ_CODEARRAY];
            memset(str, 0, sizeof(str));
            int num = 0;
            while (fscanf(fp, "%s\n", str) != EOF)
            {
                num++;
                Key key(str);
                Val val = getVal();
                add(getMap(i), key, val);
            }
            cout << "load table:" << name << " set_size:" << num << endl;
            fclose(fp);
        }

        if(!done) {
            cout << "genMap begin" << endl;
            genMap();
            cout << "genMap end" << endl;
        }
    }

    void genMap() {
        // first clean
        cleanMap();

        /*
        title{
            0,					// 混
            0,0,0,0,0,0,0,		// 风字牌1-7
            0,0,0,0,0,0,0,0,0,	// 万8-16
            0,0,0,0,0,0,0,0,0,	// 条17-25
            0,0,0,0,0,0,0,0,0,	// 筒26-34
            0,0,0,0,0,0,0,0		// 花35-42
        };
        */
        int title [MAX_MJ_CODEARRAY];
        memset(title, 0, sizeof(title));
        for (int i = 1; i <= 34; ++i) {
            title[i] = 2;
            Key key = getKey(title, MAX_MJ_CODEARRAY);
            Val val = getVal();
            m_dJiang.insert(make_pair(key,val));
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dJiang, "jiang");

        memset(title, 0, sizeof(title));
        for (int i = 8; i <= 32; i++) {
            title[i] = 1;
            title[i+1] = 1;
            title[i+2] = 1;
            Key key = getKey(title, MAX_MJ_CODEARRAY);
            Val val = getVal();
            //cout << "index:" << i << " shunzi key:" << key << endl;
            m_dShunzi.insert(make_pair(key,val));
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dShunzi, "shunzi");

        memset(title, 0, sizeof(title));
        for (int i = 1; i <= 34; ++i) {
            title[i] = 3;
            Key key = getKey(title, MAX_MJ_CODEARRAY);
            Val val = getVal();
            m_dKezi.insert(make_pair(key,val));
            //cout << "index:" << i << " kezi key:" << key << endl;
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dKezi, "kezi");

        memset(title, 0, sizeof(title));
        for (int i = 1; i <= 34; ++i) {
            title[i] = 4;
            Key key = getKey(title, MAX_MJ_CODEARRAY);
            Val val = getVal();
            m_dGang.insert(make_pair(key,val));
            //cout << "index:" << i << " gang key:" << key << endl;
            memset(title, 0, sizeof(title));
        }
        dumpTest(m_dGang, "gang");
    }

    void dump(){
        for (int i = 0; i <= MAX_HUN_COUNT; i++) {
            dumpFor(i);
        }
    }
    void dumpFor(int count){
        if (count < 0 || count > MAX_HUN_COUNT)
            return;

        char name[256];
        sprintf(name, "tbl/table_%d.tbl", count);
        FILE *fp = fopen(name, "wb+");

        tbMap tbl = getMap(count);

        for (tbMap::iterator ite = tbl.begin(); ite != tbl.end(); ite++) {
            Key key = ite->first;
            fprintf(fp, "%s\n", key.c_str());
        }
        fclose(fp);
    }
    void dumpTest(tbMap &d, string tpe){
        char name[256];
        sprintf(name, "tbl/table_%s.tbl", tpe.c_str());
        FILE *fp = fopen(name, "wb+");
        if (fp == NULL) {
            cout << "can't open file " << name << endl;
            return;
        }

        tbMap tbl = d;

        for (tbMap::iterator ite = tbl.begin(); ite != tbl.end(); ite++) {
            Key key = ite->first;
            fprintf(fp, "%s\n", key.c_str());
        }
        fclose(fp);
    }
    bool loadOf(tbMap &d, string path) {
        char name[256];
        sprintf(name, "tbl/table_%s.tbl", path.c_str());
        FILE *fp = fopen(name, "rb");
        if (fp == NULL) {
            cout << "can't load table:" << name << endl;
            return false;
        }

        char str[MAX_MJ_CODEARRAY];
        memset(str, 0, sizeof(str));
        int num = 0;
        while (fscanf(fp, "%s\n", str) != EOF) {
            num++;
            Key key(str);
            Val val = getVal();
            add(d, key, val);
        }
        cout << "load table:" << name << " set_size:" << num << endl;
        fclose(fp);
        return true;
    }

    void gen() {
        /*
            C34/4 jiang
            C34/4 + A21/0	kezi+shunzi
            C34/3 + A21/1
            C34/2 + A21/2
            C34/1 + A21/3
         */
        int tmp[MAX_MJ_CODEARRAY];
        memset(tmp, 0, sizeof(tmp));
        tbMap::iterator iteJiang = m_dJiang.begin();
        for (iteJiang; iteJiang != m_dJiang.end(); ++iteJiang) {
            tmp[0] = 0;
            tbMap::iterator iteShun = m_dShunzi.begin();
            tbMap::iterator iteKe = m_dKezi.begin();
        }
    }

    // return map index of key
    Key getIndex(tbMap &d, int index) {
        int i = 0;
        tbMap::iterator ite = d.begin();
        for (ite; ite != d.end(); ite++) {
            i++;
            if(index == i) {
                return ite->first;
            }
        }
        return "";
    }

    // key add operation
    Key KeyOpAdd(Key k1, Key k2) {
        int arr[MAX_MJ_CODEARRAY];
        memset(arr, 0, sizeof(arr));
        for (int i = 0; i < k1.length(); ++i) {
            string str1,str2;
            str1 = k1[i];
            str2 = k2[i];
            arr[i] = atoi(str1.c_str()) + atoi(str2.c_str());
            //cout << "str1:" << str1 << " str2:" << str2 << endl;
        }
        Key k3 = "";
        k3 = getKey(arr,MAX_MJ_CODEARRAY);
        return k3;
    }

    bool valid(Key k) {
        for (int i = 0; i < k.length(); ++i) {
            string str;
            str = k[i];
            int num = atoi(str.c_str());
            if (num > 4) {
                return false;
            }
        }

        return true;
    }

    void chooseFrom(tbMap &d, tbMap &tbTmp, int choose, int from) {
        if (choose <= 0 || choose > 4)
            return;

        tbTmp.clear();

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
        do {
            ++count;
            int ch = 0;
            Key chooseKey[choose];
            for (size_t i = 0; i < selectors.size(); ++i) {
                if (selectors[i]>0) {
                    //cout << values[i] << ", ";
                    Key key = getIndex(d,values[i]);
                    chooseKey[ch] = key;
                    ch++;
                }
            }
            //cout << endl;

            Key tmp = chooseKey[0];
            for (int j = 1; j < choose; j++) {
                tmp = KeyOpAdd(tmp, chooseKey[j]);
            }

            if (valid(tmp)) {
                tbTmp.insert(make_pair(tmp,getVal()));
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

    // case ke0 shun4
    void case04(Key &key) {
        chooseFrom(m_dShunzi, m_dChooseShunzi, 4, m_dShunzi.size());
        for (tbMap::iterator iteShunzi = m_dChooseShunzi.begin(); iteShunzi!= m_dChooseShunzi.end(); iteShunzi++) {
            Key tmp = KeyOpAdd(key,iteShunzi->first);
            if (valid(tmp)) {
                //cout << tmp << " 0ke 4shun key" << endl;
                m_dTmp.insert(make_pair(tmp,getVal()));
            } else {
            }
        }
    }
    void caseKeShun(Key &key, int countKe, int countShun) {
        chooseFrom(m_dKezi, m_dChooseKezi, countKe, m_dKezi.size());
        //cout << "chooseKezi size:" << m_dChooseKezi.size() << endl;
        for (tbMap::iterator iteKezi = m_dChooseKezi.begin(); iteKezi != m_dChooseKezi.end(); iteKezi++) {
            Key tmp = KeyOpAdd(key,iteKezi->first);
            if (valid(tmp)) {
                //cout << key << " jiangKey" << endl;
                //cout << iteKezi->first << " keKey" << endl;
                //cout << tmp << " jiang+ke" << endl;
                chooseFrom(m_dShunzi, m_dChooseShunzi, countShun, m_dShunzi.size());
                if (countShun <= 0) {
                    m_dTmp.insert(make_pair(tmp, getVal()));
                } else {
                    //cout << "chooseShunzi size:" << m_dChooseShunzi.size() << endl;
                    for (tbMap::iterator iteShunzi = m_dChooseShunzi.begin(); iteShunzi != m_dChooseShunzi.end(); iteShunzi++) {
                        Key tmp2 = KeyOpAdd(tmp, iteShunzi->first);
                        if(valid(tmp2)) {
                            //cout << iteShunzi->first << " shunKey" << endl;
                            m_dTmp.insert(make_pair(tmp2,getVal()));
                            //cout << tmp2 << " " << countKe << "ke " << countShun << "shun key" << endl;
                            //break;
                        }
                    }
                }
                //break;
            }
        }
    }
    void create() {
        // jiang 1
        // kezi 4 3 2 1 0
        // shun 0 1 2 3 4
        int count = 0;
        double timeUse = 0;
        struct timeval start;
        struct timeval end;
        gettimeofday(&start,NULL);
        cout << "process jiang No." << flush;
        chooseFrom(m_dJiang, m_dChooseJiang, 1, m_dJiang.size());
        //cout << "chooseJiang size:" << m_dChooseJiang.size() << endl;
        for (tbMap::iterator iteJiang = m_dChooseJiang.begin(); iteJiang != m_dChooseJiang.end(); iteJiang++) {
            count++;
            cout << " " << count << flush;
            Key key = iteJiang->first;
            caseKeShun(key, 4, 0);
            caseKeShun(key, 3, 1);
            caseKeShun(key, 2, 2);
            caseKeShun(key, 1, 3);
            case04(key);

            // for test tobe rm
            //if (count == 1) break;
        }
        gettimeofday(&end,NULL);
        timeUse = (end.tv_sec-start.tv_sec)/60.0;
        cout << "\ngen map use:" << timeUse << "m" << endl;

        gettimeofday(&start, NULL);
        cout << "begin dump file..." << endl;
        dumpTest(m_dTmp, "total");
        gettimeofday(&end,NULL);
        timeUse = end.tv_sec-start.tv_sec;
        cout << "end dump file use:" << timeUse << "s" << endl;

        cout << "total:" << m_dTmp.size() << endl;
    }

    bool check(Key key) {
        tbMap::iterator ite = m_dTmp.find(key);
        if (ite == m_dTmp.end()) {
            return false;
        }
        return true;
    }
//private:
public:
    tbMap m_dMapTb[MAX_HUN_COUNT+1];
    int m_dTotal; 		// 总胡牌数
    int m_dCollision; 	// mapKey总碰撞数
    tbMap m_dJiang;
    tbMap m_dShunzi;
    tbMap m_dKezi;
    tbMap m_dGang;

    tbMap m_dTmp;
    tbMap m_dChooseJiang;
    tbMap m_dChooseShunzi;
    tbMap m_dChooseKezi;
    tbMap m_dChooseGang;
};

void benchmark(MjMapTb &tb) {
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
        Key key = tb.getKey(source+n * 43, 43);
//		cout << "check key:" << key << endl;
        hu += tb.check(key);
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

    MjMapTb mt;
    cout << "begin load map..." << endl;
    mt.loadOf(mt.m_dTmp, "total");

    gettimeofday(&end,NULL);
    timeUse = end.tv_sec-start.tv_sec;
    cout << "load map use:" << timeUse << "s" << endl;

    cout << "begin benchmark..." << endl;
    benchmark(mt);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "usage:\n"
                "1,gen map to file.\n"
                "2,benchmark." << endl;
        return 0;
    }

    int tpe = atoi(argv[1]);
    if (tpe==1) {
        // gen map
        MjMapTb mt;
        mt.load();
        mt.create();
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

