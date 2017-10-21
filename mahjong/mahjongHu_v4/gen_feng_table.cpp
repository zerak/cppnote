#include "table_mgr.h"
#include <stdio.h>
#include <set>
#include <map>
#include <sys/time.h>

#include <iostream>
using namespace std;

std::map<int, bool> gui_tested[9];
std::map<int, bool> gui_eye_tested[9];

bool check_add(int cards[], int gui_num, bool eye, int tpe) {
    int key = 0;
    int len = (tpe == 1) ? 7 : 9;
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

    for (int i=0; i<9; i++){
        if (cards[i] > 4) {
            return true;
        }
    }
    TableMgr::add(key, gui_num, eye, false);
    return true;
}

void parse_table_sub(int cards[], int num, bool eye) {
    for (int i=0; i<9; i++) {
        if (cards[i] == 0) {
             continue;
        }

        cards[i]--;

        if (!check_add(cards, num, eye, 1)) {
            cards[i]++;
            continue;
        }

        if (num < 8) {
            parse_table_sub(cards, num + 1, eye);
        }
        cards[i]++;
    }
}

void parse_table(int cards[], bool eye) {
    if (!check_add(cards, 0, eye, 1)) {
        return;
    }
    parse_table_sub(cards, 1, eye);
}

void gen_auto_table_sub(int cards[], int level, bool eye) {
    for(int i=0;i<7;++i) {
        if(cards[i] > 3)
            continue;

        cards[i] += 3;

        parse_table(cards, eye);
        if(level<4) {
            gen_auto_table_sub(cards, level + 1, eye);
        }
        cards[i] -= 3;
    }
}

void genFengSub(int *cards, int level, bool hasJiang) {
    for(int i=0; i < 7; ++i) {
        if(cards[i] > 3)
            continue;

        cards[i] += 3;

        parse_table(cards, hasJiang);
        if(level<4) {
            genFengSub(cards, level + 1, hasJiang);
        }
        cards[i] -= 3;
    }
}
void genFengKe() {
    int card[9] = {0};
    genFengSub(card, 1, false);
}
void genFengJiangKe() {
    int cards[34] = {0};
    for(int i=0; i<7; ++i) {
        cards[i] = 2;
        parse_table(cards, true);
        genFengSub(cards, 1, true);
        cards[i] = 0;
    }
}

void gen_table() {
    int cards[9] = {0};
    gen_auto_table_sub(cards, 1, false);
}
void gen_eye_table() {
    int cards[34] = {0};
    for(int i=0; i<7; ++i) {
        cards[i] = 2;
        parse_table(cards, true);
        gen_auto_table_sub(cards, 1, true);
        cards[i] = 0;
    }
}

int main() {
//    printf("generate feng table begin...\n");

    double timeUse = 0;
    struct timeval start, end;
//    gettimeofday(&start, 0);
//    // func
//    gen_table();
//    gen_eye_table();
//    gettimeofday(&end, 0);
    TableMgr::init();
//    timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
//    cout << "gen 0 map cost:" << timeUse << "us" << endl;

    gettimeofday(&start, 0);
    genFengKe();
    genFengJiangKe();
    gettimeofday(&end, 0);
    timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
    cout << "gen 1 map cost:" << timeUse << "us" << endl;

    cin >> timeUse;

    TableMgr::dump_feng_table();
//    printf("generate feng table end...\n");

    return 0;
}


