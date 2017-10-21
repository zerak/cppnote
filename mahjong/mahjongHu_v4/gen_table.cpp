#include "table_mgr.h"
#include <stdio.h>
#include <set>
#include <map>
#include <sys/time.h>
#include <iostream>
using namespace std;

std::map<int, bool> gui_tested[9];
std::map<int, bool> gui_eye_tested[9];

bool check_add(int cards[], int gui_num, bool eye) {
    int key = 0;

    for (int i=0; i<9; i++) {
        key = key * 10 + cards[i];
    }

    if (key == 0) {
        return false;
    }

    std::map<int, bool>* m;
    if (!eye) {
        m = &gui_tested[gui_num];
    } else {
        m = &gui_eye_tested[gui_num];
    }
    if(m->find(key) != m->end()){
        return false;
    }

    (*m)[key] = true;

    for (int i=0; i<9; i++) {
        if (cards[i] > 4) {
            return true;
        }
    }
    TableMgr::add(key, gui_num, eye, true);
    return true;
}

static void parse_table_sub(int cards[], int num, bool eye) {
    for (int i=0; i<9; i++) {
        if (cards[i] == 0) {
             continue;
        }

        cards[i]--;

        if (!check_add(cards, num, eye)) {
            cards[i]++;
            continue;
        }

        if (num < 8) {
            parse_table_sub(cards, num + 1, eye);
        }
        cards[i]++;
    }
}

static void parse_table(int cards[], bool eye) {
    if (!check_add(cards, 0, eye)) {
        return;
    }
    parse_table_sub(cards, 1, eye);
}

void gen_auto_table_sub(int cards[], int level, bool eye) {
    for(int i=0;i<16;++i) {
        if(i <= 8) {
            if(cards[i] > 3)
                continue;
            cards[i] += 3;
        } else {
            int index = i - 9;
            if(cards[index]>5 || cards[index+1]>5 || cards[index+2]>5)
                continue;
            cards[index] += 1;
            cards[index+1] += 1;
            cards[index+2] += 1;
        }

        parse_table(cards, eye);
        if(level<4) {
            gen_auto_table_sub(cards, level + 1, eye);
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

void genSanSeSub(int *cards, int len, int level, bool hasJiang) {
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

        parse_table(cards, hasJiang);
        if(level < 4) {
            genSanSeSub(cards, len, level + 1, hasJiang);
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
    genSanSeSub(cards, 9, 1, false);
}
void genSanSeJiangKeShun() {
    int cards[34] = {0};
    for(int i = 0; i < 9; ++i) {
        cout << "jiang No." << i << endl;
        cards[i] = 2;
        parse_table(cards, true);
        genSanSeSub(cards, 9, 1, true);
        cards[i] = 0;
    }
}

void gen_table() {
    int cards[34] = {0};
    gen_auto_table_sub(cards, 1, false);
}
void gen_eye_table() {
    int cards[34] = {0};

    for(int i=0; i<9; ++i) {
        printf("eye %d\n",i);
        cards[i] = 2;
        parse_table(cards, true);
        gen_auto_table_sub(cards, 1, true);
        cards[i] = 0;
    }
}

int main() {
    TableMgr::init();
//    printf("generate feng table begin...\n");

    double timeUse = 0;
    struct timeval start, end;
    gettimeofday(&start, 0);
    // func
    gen_table();
    gen_eye_table();
    gettimeofday(&end, 0);
    timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
    cout << "gen 0 map cost:" << timeUse << "us" << endl;
    TableMgr::dump_table();

    gettimeofday(&start, 0);
    genSanSeKeShun();
    genSanSeJiangKeShun();
    gettimeofday(&end, 0);
    timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
    cout << "gen 1 map cost:" << timeUse << "us" << endl;

//    cin >> timeUse;
//    TableMgr::dump_feng_table();
//    printf("generate feng table end...\n");

    return 0;
}