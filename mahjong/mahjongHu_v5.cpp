
//#include <stdlib.h> // itoa
#include <stdio.h>
#include <unistd.h> // sleep
#include <cstdlib>	// srand
#include <iostream>
#include <string.h>
#include <algorithm>
#include <map>
#include <sys/time.h>
using namespace std;

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
		// init table gen data
		genMap();

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
		return;
	}
	void load() {
		// load table
		for (int i = 0; i < MAX_HUN_COUNT; ++i) {
			loadTb(FengZi, i, true);
			loadTb(FengZi, i, false);

			loadTb(SanSe, i, true);
			loadTb(SanSe, i, false);
		}
	}

	// handList: 当前手牌
	// len: handList长度
	// mjCcodeIdx: 最后拿的牌index
	// hunCount: 混个数
	bool checkHu(int *handList, int len) {
		int hunCount = handList[0];
		return false;
	}

private:
public: // todo
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
		tbMap::iterator ite = tm.find(key);
		if (ite != tm.end()) {
			return false;
		}
		tm.insert(make_pair(key, val));
		return true;
	}
	bool check_add(int *cards, int len, int hunNum, bool hasJiang, MjType tpe) {
		int key = 0;
		for (int i = 0; i < len; i++) {
			key = key * 10 + cards[i];
		}

		if (key == 0) {
			return false;
		}

		for (int i = 0; i < len; i++) {
			if (cards[i] > 4) {
				return true;
			}
		}

		Val val = getVal();
		return add(getMap(tpe, hunNum, hasJiang), key, val);
	}
	void parse_table_sub(int *cards, int len, int hunNum, bool hasJiang, MjType tpe) {
		for (int i = 0; i < len; i++) {
			if (cards[i] == 0) {
				continue;
			}

			cards[i]--;

			if (!check_add(cards, len, hunNum, hasJiang, tpe)) {
				cards[i]++;
				continue;
			}

			if (hunNum < MAX_HUN_COUNT) {
				parse_table_sub(cards, len, hunNum + 1, hasJiang, tpe);
			}
			cards[i]++;
		}
	}
	void parse_table(int *cards, int len, bool hasJiang, MjType tpe) {
		if (!check_add(cards, len, 0, hasJiang, tpe)) {
			return;
		}
//		parse_table_sub(cards, len, 1, hasJiang,tpe);
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

			parse_table(cards, len, hasJiang, SanSe);
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
			parse_table(cards, 9, true, SanSe);
			genSanSeSub(cards, 9, 1, true);
			cards[i] = 0;
		}
	}
	void genSanSeMap() {
		cout << "begin gen sanse ke/shun..." << endl;
		genSanSeKeShun();	// todo
		cout << "sanse ke/shun gen end" << endl;

		cout << "begin gen sanse jiang + ke/shun..." << endl;
		genSanSeJiangKeShun();
		cout << "sanse jiang + ke/shun gen end" << endl;
	}

	void genFengSub(int *cards, int len, int level, bool hasJiang) {
		for(int i=0; i < len; ++i) {
			if(cards[i] > 3)
				continue;

			cards[i] += 3;

			parse_table(cards, len, hasJiang, FengZi);
			if(level<4) {
				genFengSub(cards, len, level + 1, hasJiang);
			}
			cards[i] -= 3;
		}
	}
	void genFengKe() {
		int card[7] = {0};
		genFengSub(card, 7, 1, false);
	}
	void genFengJiangKe() {
		int cards[7] = {0};
		for(int i=0; i<7; ++i) {
			cards[i] = 2;
			parse_table(cards, 7, true, FengZi);
			genFengSub(cards, 7, 1, true);
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
		genSanSeMap();		// san se map
		gettimeofday(&end, NULL);
		timeUse = end.tv_sec - start.tv_sec;
		cout << "gen sanse map cost:" << timeUse << "s" << endl;

		gettimeofday(&start, NULL);
//		genFengMap();	// feng map	todo
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
		if (tpe == SanSe) {
			if(hasJiang) {
				return "tbl/table_sanse_jiang_" + itos(hunNum);
			}
			return "tbl/table_sanse_" + itos(hunNum);
		} else {
			if (hasJiang) {
				return "tbl/table_feng_jiang_" + itos(hunNum);
			}
			return "tbl/table_feng_" + itos(hunNum);
		}
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
private:
	tbMap m_dFinalSanSe[MAX_HUN_COUNT+1];		// 最终存放带混三色数据
	tbMap m_dFinalSanSeJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混三色数据

	tbMap m_dFinalFeng[MAX_HUN_COUNT+1];		// 最终存放带混风字数据
	tbMap m_dFinalFengJiang[MAX_HUN_COUNT+1];	// 最终存放带将、混风字数据
};

void print_cards(int* cards) {
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
void benchmark(MjMapTbV5& tb) {
#define MAX_MJ_CODEARRAY 35
	int MAX_COUNT = 100 * 10000;
	int source[MAX_COUNT * 9 * MAX_MJ_CODEARRAY];
	int allCards[136];
	memset(source, 0, sizeof(source));
	memset(allCards, 0, sizeof(allCards));
	int code = 0;
	for (int i = 0; i < 34; i++) {
		allCards[i * 4] = code + 1;
		allCards[i * 4 + 1] = code + 1;
		allCards[i * 4 + 2] = code + 1;
		allCards[i * 4 + 3] = code + 1;
		code++;
	}

	int hu = 0;
	int total = 0;
	int hunCount = 4;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	srand(1);
	cout << "====shuffle begin" << endl;

	gettimeofday(&start, NULL);
	for (int n = 0; n < MAX_COUNT; ++n) {
		random_shuffle(allCards, allCards + 136);
		for (int i = 0; i < 9; ++i) { // 136/14 -> 9
			int* cards = &source[total++ * MAX_MJ_CODEARRAY];
			memset(cards, 0, MAX_MJ_CODEARRAY);
			for (int j = i * 14; j < i * 14 + 14 - hunCount; j++)
				++cards[allCards[j]];
			cards[0] = hunCount;
		}
	}
	gettimeofday(&end, NULL);
	timeUse = end.tv_sec - start.tv_sec;
	cout << "====shuffle end cost:" << timeUse << "s" << endl;

	cout << "====begin benchmark..." << endl;
	// hu check
	gettimeofday(&start, NULL);
	for (int n = 0; n < total; ++n) {
		//sleep(1);
//        hu += tb.checkHu(source + n * 43, 43, hunCount);
		if (tb.checkHu(source + n * MAX_MJ_CODEARRAY, MAX_MJ_CODEARRAY)) {
//            print_cards(source + n * MAX_MJ_CODEARRAY);
			++hu;
		} else {
			//print_cards(source + n * MAX_MJ_CODEARRAY);
		}
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	cout << "check:" << total/10000 << "w hun:" << hunCount << " hu:" << hu << " cost:" << timeUse << "ms" << endl;
	cout << "====benchmark end" << endl;
}
void checkV1(MjMapTbV5& t) {
	cout << "====begin checkV1 ..." << endl;
	// 原手牌格式
	int cards[] = {
			3,                            // 混个数
			1, 0, 0, 0, 0, 0, 1,          // 风字牌1-7
			0, 0, 0, 0, 0, 0, 0, 0, 3,    // 万8-16
			0, 0, 0, 0, 0, 0, 3, 0, 3,    // 条17-25
			0, 0, 0, 0, 0, 0, 0, 0, 0,    // 筒26-34
			0, 0, 0, 0, 0, 0, 0, 0        // 花35-42
	};

	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	bool hu = t.checkHu(cards, sizeof(cards) / sizeof(cards[0]));
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec - start.tv_usec);
	cout << "====checkV1 hu?:" << hu << " cost:" << timeUse << "us" << endl;
}
int main(int argc, char **argv) {
	MjMapTbV5 t;
	t.init();
	int a;
	cin >> a;

	if (argc < 2) {
		cout << "USAGE:\n"
				"1,benchmark." << endl;
		return 0;
	}

	int tpe = atoi(argv[1]);
	if (tpe == 1) {
		benchmark(t);
		//int tmp;
		//cin >> tmp;
	}
	else {
		cout << "arg err check usage." << endl;
		cout << "1,benchmark." << endl;
	}

	return 0;
}
