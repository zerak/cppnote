

#include "table_mgr.h"
#include <stdio.h>
#include <set>
#include <map>
#include <sys/time.h>

#include <iostream>
using namespace std;

class Tmp {
public:
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
};

class Tmp2 {
	enum MjType {
		SanSe = 0,
		FengZi = 1
	};

#define MAX_HUN_COUNT 8
	typedef int Key;
	typedef bool Val;
	typedef map<Key, Val> tbMap;

public:
    std::map<int, bool> gui_tested[9];
    std::map<int, bool> gui_eye_tested[9];

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
	bool add(tbMap *tm, Key key, Val val) {
		tm->insert(make_pair(key, val));
		return true;
	}
    bool check_add2(int cards[], int gui_num, bool eye) {
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
        Val val = true;
        return add(&getMap(SanSe, gui_num, eye), key, val);
    }
    void parse_table_sub2(int cards[], int num, bool eye) {
        for (int i=0; i<9; i++) {
            if (cards[i] == 0) {
                continue;
            }

            cards[i]--;

            if (!check_add2(cards, num, eye)) {
                cards[i]++;
                continue;
            }

            if (num < 8) {
                parse_table_sub2(cards, num + 1, eye);
            }
            cards[i]++;
        }
    }

    void parse_table2(int cards[], bool eye) {
        if (!check_add2(cards, 0, eye)) {
            return;
        }
        parse_table_sub2(cards, 1, eye);
    }

    void gen_auto_table_sub2(int cards[], int level, bool eye) {
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

            parse_table2(cards, eye);
            if(level<4) {
                gen_auto_table_sub2(cards, level + 1, eye);
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

            parse_table2(cards, hasJiang);
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
            parse_table2(cards, true);
            genSanSeSub(cards, 9, 1, true);
            cards[i] = 0;
        }
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

    void dump() {
        // dump table
		for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
			dumpFor(m_dFinalFeng[i], getTbName(FengZi, i, false));
			dumpFor(m_dFinalFengJiang[i], getTbName(FengZi, i, true));

			dumpFor(m_dFinalSanSe[i], getTbName(SanSe, i, false));
			dumpFor(m_dFinalSanSeJiang[i], getTbName(SanSe, i, true));
		}

        // print info
        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
            cout << "fengKe hun:" << i << " size:" << m_dFinalFeng[i].size() << endl;
        }
        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
            cout << "fengJiangKe hun:" << i << " size:" << m_dFinalFengJiang[i].size() << endl;
        }
        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
            cout << "sanSe hun:" << i << " size:" << m_dFinalSanSe[i].size() << endl;
        }
        for (int i = 0; i < MAX_HUN_COUNT+1; ++i ) {
            cout << "sanseJiang hun:" << i << " size:" << m_dFinalSanSeJiang[i].size() << endl;
        }
    }

private:
    tbMap m_dFinalSanSe[MAX_HUN_COUNT+1];		// 最终存放带混三色数据
    tbMap m_dFinalSanSeJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混三色数据

    tbMap m_dFinalFeng[MAX_HUN_COUNT+1];		// 最终存放带混风字数据
    tbMap m_dFinalFengJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混风字数据
};

int main() {
    TableMgr::init();

    Tmp2 t2;
    double timeUse = 0;
    struct timeval start, end;
    gettimeofday(&start, 0);
    t2.genSanSeKeShun();
    t2.genSanSeJiangKeShun();
    gettimeofday(&end, 0);
    //timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
    timeUse = (end.tv_sec - start.tv_sec) * 1000;
    cout << "gen sanse map cost:" << timeUse << "ms" << endl;
    t2.dump();

    Tmp t;
    gettimeofday(&start, 0);
    t.genFengKe();
    t.genFengJiangKe();
    gettimeofday(&end, 0);
    //timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec-start.tv_usec);
    timeUse = (end.tv_sec - start.tv_sec) * 1000;
    cout << "gen feng map cost:" << timeUse << "ms" << endl;

    cin >> timeUse;
    TableMgr::dump_feng_table();

    return 0;
}

