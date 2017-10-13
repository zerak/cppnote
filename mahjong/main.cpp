
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>
using namespace std;

typedef string Key;
typedef string Val;
typedef map<Key,Val> tbMap;

void genMap();
void dump();
void dumpFor(int count);
void dumpTest(tbMap &d, string tpe);
void gen();

#define MAX_HUN_COUNT 8
#define MAX_MJ_CODEARRAY 43

tbMap gMap[MAX_HUN_COUNT+1];
int total = 0;
int collision = 0;
int tmp[MAX_MJ_CODEARRAY];
tbMap jiang;
tbMap shunzi;
tbMap kezi;

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

tbMap& getMap(int count) {
    return gMap[count];
}

bool add(tbMap &tester, Key &key, Val &val) {
    tbMap::iterator ite = tester.find(key);
    if (ite != tester.end()) {
        return false;
    }
    tester.insert(make_pair(key,val));
    cout << "insert map key:" << key << endl;
    return true;
}

void load() {
    bool done = true;
    char name[256];
    for (int i = 0; i <= MAX_HUN_COUNT; i++)
    {
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

    // fot test
    // tobe rm
    gMap[8] = gMap[0];
    dumpFor(8);
}

void dumpFor(int count) {
    if (count < 0 || count > 8)
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

void dump() {
    for (int i = 0; i <= 8; i++) {
        dumpFor(i);
    }
}

int main() {
//    load();
    genMap();
    return 0;
    memset(tmp, 0, sizeof(tmp));
    for( int hunCount = 0; hunCount <= 0; hunCount++) {
        tmp[0] = hunCount;
        for(int i = 1; i < 43; i++) {
            for( int k = 0; k <= 4; k++) {
                total++;
                tmp[i] = k;
                for(int w = 0; w < sizeof(tmp)/sizeof(int); w++){
                    cout << tmp[w];
                }
                cout << endl;
                Key key = getKey(tmp, sizeof(tmp)/sizeof(int));
                Val val = getVal();
                if (!add(getMap(hunCount), key, val)) {
                    collision++;
                    cout<< "key:" << key << " collision:" << tmp << endl;
                }

            }
        }
    }

    dump();

    printf("total:%d collision:%d\n",total,collision);

    memset(tmp, 0, sizeof(tmp));
    tmp[0] = 0;
    tmp[1] = 0;		// 东
    tmp[2] = 0;		// 南
    tmp[3] = 0;		// 西
    tmp[4] = 0;		// 北
    tmp[5] = 0;		// 中
    tmp[6] = 0;		// 发
    tmp[7] = 0;		// 白
    tmp[8] = 1;		// 一万
    tmp[9] = 1;
    tmp[10] = 1;
    tmp[11] = 1;
    tmp[12] = 1;
    tmp[13] = 1;
    tmp[14] = 2;
    tmp[15] = 2;
    tmp[16] = 3; 	// 九万
    tmp[17] = 0;	// 一条
    tmp[18] = 0;
    tmp[19] = 0;
    tmp[20] = 0;
    tmp[21] = 0;
    tmp[22] = 0;
    tmp[23] = 0;
    tmp[24] = 0;
    tmp[25] = 0;	// 九条
    tmp[26] = 0;	// 一筒
    memset(tmp, 0, sizeof(tmp));
    tmp[1] = 1;
//	long long key = getKeyV2(tmp, 43);
//	cout<< "key:" << key << endl;
    cout<< "sizeof(tmp)/sizeof(int):" << sizeof(tmp)/sizeof(int) << endl;

    int a;
    cin >> a;
    return 0;
}

/*
title				{
	0,					// 混
	0,0,0,0,0,0,0,		// 风字牌1-7
	0,0,0,0,0,0,0,0,0,	// 万8-16
	0,0,0,0,0,0,0,0,0,	// 条17-25
	0,0,0,0,0,0,0,0,0,	// 筒26-34
	0,0,0,0,0,0,0,0		// 花35-42
};
*/
void genMap() {
    int title [MAX_MJ_CODEARRAY];
    memset(title, 0, sizeof(title));
    for (int i = 1; i <= 34; ++i) {
        title[i] = 2;
        Key key = getKey(title, MAX_MJ_CODEARRAY);
        Val val = getVal();
        jiang.insert(make_pair(key,val));
        memset(title, 0, sizeof(title));
    }
    dumpTest(jiang, "jiang");

    memset(title, 0, sizeof(title));
    for (int i = 8; i <= 34; i++) {
        title[i] = 1;
        title[i+1] = 1;
        title[i+2] = 1;
        Key key = getKey(title, MAX_MJ_CODEARRAY);
        Val val = getVal();
        cout << "index:" << i << " shunzi key:" << key << endl;
        shunzi.insert(make_pair(key,val));
        memset(title, 0, sizeof(title));
    }
    dumpTest(shunzi, "shunzi");

    memset(title, 0, sizeof(title));
    for (int i = 1; i <= 34; ++i) {
        title[i] = 3;
        Key key = getKey(title, MAX_MJ_CODEARRAY);
        Val val = getVal();
        kezi.insert(make_pair(key,val));
        cout << "index:" << i << " kezi key:" << key << endl;
        memset(title, 0, sizeof(title));
    }
    dumpTest(kezi, "kezi");

    for (int i = 1; i <= 34; ++i) {
        title[i] = 4;
        Key key = getKey(title, MAX_MJ_CODEARRAY);
        Val val = getVal();
        kezi.insert(make_pair(key,val));
        cout << "index:" << i << " kezi key:" << key << endl;
        memset(title, 0, sizeof(title));
    }
    dumpTest(kezi, "kezi");
}

void dumpTest(tbMap &d, string tpe) {
    char name[256];
    sprintf(name, "tbl/table_%s.tbl", tpe.c_str());
    FILE *fp = fopen(name, "wb+");

    tbMap tbl = d;

    for (tbMap::iterator ite = tbl.begin(); ite != tbl.end(); ite++) {
        Key key = ite->first;
        fprintf(fp, "%s\n", key.c_str());
    }
    fclose(fp);
}

/*
	C34/4
	C34/3 + A21/1
	C34/2 + A21/2
	C34/1 + A21/3

	3*4 + 2
	3N + 2
 */
void gen() {
    tbMap::iterator iteJiang = jiang.begin();
    for (int i = 0; i < jiang.size(); ++i) {

    }
    tbMap::iterator iteShun = shunzi.begin();
    tbMap::iterator iteKe = kezi.begin();
}
