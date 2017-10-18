
//#include <stdlib.h> // itoa
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
#include <numeric>  // accumulate
using namespace std;

class MjMapTbV2 {
private:
#define MAX_HUN_COUNT 8
	typedef int Key;
	typedef int Val;
	typedef map<Key, Val> tbMap;
	typedef vector<Key> tbVec;
public:
	MjMapTbV2() {}
	~MjMapTbV2() {}

public:
	enum MjType {
		SanSe = 0,
		FengZi = 1
	};

	Val getVal() {
		return Val(0);
	}

	tbMap& getMap(MjType tpe, int hunCount) {
		if (tpe == SanSe) {
			return m_dFinalSanSeHun[hunCount];
		}

		return m_dFinalFengHun[hunCount];
	}

	void add(tbMap &tm, Key &key, Val &val) {
		tbMap::iterator ite = tm.find(key);
		if (ite != tm.end()) {
			return;
		}
		tm.insert(make_pair(key, val));
	}

	string getTbName(MjType tpe, int hunCount) {
		if (tpe == SanSe) {
			return "sanse" + itos(hunCount);
		}

		return "feng" + itos(hunCount);
	}

	bool loadTb(MjType tpe, int hunCount) {
		char name[256];
		sprintf(name, "tbl/table_%s.tbl", getTbName(tpe, hunCount).c_str());
		FILE *fp = fopen(name, "rb");
		if (fp == NULL) {
			cout << "can't load table:" << name << endl;
			return false;
		}

		int num = 0;
		int key = 0;
		while (fscanf(fp, "%d\n", &key) != EOF) {
			Val val = getVal();
			add(getMap(tpe, hunCount), key, val);
			num++;
		}
		cout << "load table:" << name << " " << num << "times, size:" << getMap(tpe, hunCount).size() << endl;
		fclose(fp);
		return true;
	}

	void load() {
		if (!loadTb(SanSe, 0)) {
			//genSanSeJiang();
			genSanSeKe();       // 生成三色牌Ke
			genSanSeShun();     // 生成三色牌ShunZi
			//genFengJiang();
			genFengKe();        // 生成风字牌Ke

			genSanSeMap();
			genFengMap();
			return;
		}

		cout << "sanseKe:" << m_dSanSeKe.size() << endl;
		cout << "sanseShun:" << m_dSanSeShun.size() << endl;
		cout << "fengKe:" << m_dFengKe.size() << endl;

		// begin from 1hun
		for (int i = 1; i < sizeof(m_dFinalSanSeHun) / sizeof(m_dFinalSanSeHun[0]); ++i) {
			loadTb(SanSe, i);
		}
		// load feng
		for (int j = 0; j < sizeof(m_dFinalFengHun) / sizeof(m_dFinalFengHun[0]); ++j) {
			loadTb(FengZi, j);
		}
	}

	bool valid(Key key) {
		char buf[32];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", key);
		//itoa(key, buf, 10);
		for (int i = 0; i < 32; ++i) {
			if (buf[i] - '0' > 4) {
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

		int values[from];
		for (int i = 0; i < from; ++i) {
			values[i] = i + 1;
		}

		int elements[sizeof(values) / sizeof(values[0])];
		memset(elements, 0, sizeof(elements));
		for (int i = 0; i < choose; ++i) {
			elements[i] = 1;
		}
		const size_t N = sizeof(elements) / sizeof(elements[0]);
		std::vector<int> selectors(elements, elements + N);

		int count = 0;
		Key chooseKey[choose];
		do {
			++count;
			int ch = 0;
			for (size_t i = 0; i < selectors.size(); ++i) {
				if (selectors[i] > 0) {
					//cout << values[i] << ",i:" << i << " " << d[values[i]-1];
					Key key = d[values[i] - 1];   // 获取第几个值
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
		} while (prev_permutation(selectors.begin(), selectors.end()));

		//cout << "total count:" << count << " tmMapSize:" << tbTmp.size() << endl;
	}

	Key getKey(int *list, int size) {
		Key key = 0;
		for (int i = 0; i < size; ++i) {
			key += list[i] * pow(10, size - (i + 1));
		}
		return key;
	}
	// todo
	tbVec getKey(Key key) {
		tbVec vec;
		char buf[9];
		sprintf(buf, "%d", key);
		char tmp[9];
		memset(tmp, 0, sizeof(tmp));
		for (int i = 0; i < sizeof(buf); ++i) {
			if (buf[i] - '0' > 0){
				buf[i]--;
				memcpy(tmp, buf, sizeof(tmp));
				int num = atoi(tmp);
				if (num>0) {
					vec.push_back(atoi(tmp));
				}
				buf[i]++;
			}
		}
		cout << "key:" << key << " buf:" << buf << endl;
		for (int j = 0; j < vec.size(); ++j) {
			cout << vec[j] << " ";
		}
		cout << "vec size:" << vec.size() << endl;
		return vec;
	}

	tbVec& genSanSeJiang() {
		int title[9];
		memset(title, 0, sizeof(title));
		for (int i = 0; i < 9; ++i) {
			title[i] = 2;
			Key key = getKey(title, 9);
			m_dSanSeJiang.push_back(key);
			//cout << "sanseJiang:" << key << endl;
			memset(title, 0, sizeof(title));
		}
		dumpTest(m_dSanSeJiang, "sanseJiang");
		return m_dSanSeJiang;
	}
	tbVec& genSanSeKe() {
		int title[9];
		memset(title, 0, sizeof(title));
		for (int i = 0; i < 9; ++i) {
			title[i] = 3;
			Key key = getKey(title, 9);
			m_dSanSeKe.push_back(key);
			//cout << "sanseKe:" << key << endl;
			memset(title, 0, sizeof(title));
		}
		dumpTest(m_dSanSeKe, "sanseKe");
		return m_dSanSeKe;
	}
	tbVec& genSanSeShun() {
		int title[9];
		memset(title, 0, sizeof(title));
		for (int i = 0; i < 7; i++) {
			title[i] = 1;
			title[i + 1] = 1;
			title[i + 2] = 1;
			Key key = getKey(title, 9);
			m_dSanSeShun.push_back(key);
			memset(title, 0, sizeof(title));
		}
		dumpTest(m_dSanSeShun, "sanseShun");
		return m_dSanSeShun;
	}
	tbVec& genFengJiang() {
		int title[7];
		memset(title, 0, sizeof(title));
		for (int i = 0; i < 7; ++i) {
			title[i] = 2;
			Key key = getKey(title, 7);
			m_dFengJiang.push_back(key);
			memset(title, 0, sizeof(title));
		}
		dumpTest(m_dFengJiang, "fengJiang");
		return m_dFengJiang;
	}
	tbVec& genFengKe() {
		int title[7];
		memset(title, 0, sizeof(title));
		for (int i = 0; i < 7; ++i) {
			title[i] = 3;
			Key key = getKey(title, 7);
			m_dFengKe.push_back(key);
			memset(title, 0, sizeof(title));
		}
		dumpTest(m_dFengKe, "fengKe");
		return m_dFengKe;
	}
	void dumpTest(tbVec &d, string tpe) {
		char name[256];
		sprintf(name, "tbl/table_%s.tbl", tpe.c_str());
		FILE *fp = fopen(name, "wb+");

		for (tbVec::iterator ite = d.begin(); ite != d.end(); ite++) {
			Key key = *ite;
			fprintf(fp, "%d\n", key);
		}
		fclose(fp);
	}
	void dumpTest(tbMap &d, string tpe) {
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

	string itos(int num) {
		string str;
		char buf[] = {0, 0, 0, 0};
		sprintf(buf, "%d", num);
		str = buf;
		return str;
	}
	void caseKeShun(tbVec &vecKe, tbVec &vecShun, int countKe, int countShun, tbMap& dest) {
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
					dest.insert(make_pair(tmp, getVal()));
				}
				else {
					//cout << "chooseShunzi size:" << m_dChooseShunzi.size() << endl;
					for (tbMap::iterator iteShun = tmpDataShun.begin(); iteShun != tmpDataShun.end(); iteShun++) {
						Key tmp2 = KeyOpAdd(tmp, iteShun->first);
						if (valid(tmp2)) {
							//cout << iteShunzi->first << " shunKey" << endl;
							dest.insert(make_pair(tmp2, getVal()));
							//cout << tmp2 << " " << countKe << "ke " << countShun << "shun key" << endl;
							//break;
						}
					}
				}
				//break;
			}
		}
	}
	void genSanSe1(tbMap& tb) {
		// ke1 shun0
		// ke0 shun1
		chooseFrom(m_dSanSeKe, tb, 1, m_dSanSeKe.size());    // ke c(9/1)
		chooseFrom(m_dSanSeShun, tb, 1, m_dSanSeShun.size());    // shun c(7/1)
	}
	void genSanSe2(tbMap& tb) {
		// ke2 shun0
		// ke1 shun1
		// ke0 shun2
		// 2-0
		chooseFrom(m_dSanSeKe, tb, 2, m_dSanSeKe.size());    // ke c(9/2)

		// 1-1
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 1, tb);

		// 0-2
		chooseFrom(m_dSanSeShun, tb, 2, m_dSanSeShun.size());     // shun c(7/2)
	}
	void genSanSe3(tbMap& tb) {
		// ke3 shun0
		// ke2 shun1
		// ke1 shun2
		// ke0 shun3
		// 3-0
		chooseFrom(m_dSanSeKe, tb, 3, m_dSanSeKe.size());

		// 2-1
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 2, 1, tb);

		// 1-2
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 2, tb);

		// 0-3
		chooseFrom(m_dSanSeShun, tb, 3, m_dSanSeShun.size());
	}
	void genSanSe4(tbMap& tb) {
		// ke4 shun0
		// ke3 shun1
		// ke2 shun2
		// ke1 shun3
		// ke0 shun4
		// 4-0
		chooseFrom(m_dSanSeKe, tb, 4, m_dSanSeKe.size());

		// 3-1
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 3, 1, tb);

		// 2-2
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 2, 2, tb);

		// 1-3
		caseKeShun(m_dSanSeKe, m_dSanSeShun, 1, 3, tb);

		// 0-4
		chooseFrom(m_dSanSeShun, tb, 4, m_dSanSeShun.size());
	}
	void genSanSeMap() {
		genSanSe1(m_dFinalSanSeHun[0]);
		genSanSe2(m_dFinalSanSeHun[0]);
		genSanSe3(m_dFinalSanSeHun[0]);
		genSanSe4(m_dFinalSanSeHun[0]);

		// hun
		for (int hunCount = 0; hunCount < sizeof(m_dFinalSanSeHun) / sizeof(m_dFinalSanSeHun[0]) -1; ++hunCount) {
			//cout << "sanseHun:" << hunCount << endl;
			tbMap tmp = getMap(SanSe, hunCount);
			for (tbMap::iterator ite = tmp.begin(); ite != tmp.end(); ++ite) {
				tbVec keyVec = getKey(ite->first); // 拿到Key，分别减1
				for (tbVec::iterator ite = keyVec.begin(); ite != keyVec.end(); ++ite) {
					Key key = *ite;
					m_dFinalSanSeHun[hunCount+1].insert(make_pair(key, getVal()));
				}
			}
		}

		// dumpSanSeHun
		for (int i = 0; i < sizeof(m_dFinalSanSeHun) / sizeof(m_dFinalSanSeHun[0]); ++i) {
			dumpTest(m_dFinalSanSeHun[i], "sanse"+itos(i));
			//cout << "sanseKeShun size:" << m_dFinalSanSeHun[i].size() << endl;
		}
	}

	void genFengMap() {
		chooseFrom(m_dFengKe, m_dFinalFengHun[0], 1, m_dFengKe.size());
		chooseFrom(m_dFengKe, m_dFinalFengHun[0], 2, m_dFengKe.size());
		chooseFrom(m_dFengKe, m_dFinalFengHun[0], 3, m_dFengKe.size());
		chooseFrom(m_dFengKe, m_dFinalFengHun[0], 4, m_dFengKe.size());

		// hun
		for (int hunCount = 0; hunCount < sizeof(m_dFinalFengHun) / sizeof(m_dFinalFengHun[0]) -1; ++hunCount) {
			//cout << "fengHun:" << hunCount << endl;
			tbMap tmp = getMap(FengZi, hunCount);
			for (tbMap::iterator ite = tmp.begin(); ite != tmp.end(); ++ite) {
				tbVec keyVec = getKey(ite->first); // 拿到Key，分别减1
				for (tbVec::iterator ite = keyVec.begin(); ite != keyVec.end(); ++ite) {
					Key key = *ite;
					m_dFinalFengHun[hunCount+1].insert(make_pair(key, getVal()));
				}
			}
		}

		// dumpFengHun
		for (int i = 0; i < sizeof(m_dFinalSanSeHun) / sizeof(m_dFinalSanSeHun[0]); ++i) {
			dumpTest(m_dFinalFengHun[i], "feng"+itos(i));
			//cout << "fengKe size:" << m_dFinalFengHun[i].size() << endl;
		}
	}

	void genMap() {
		genSanSeMap();
		genFengMap();
	}

	bool check(Key key, MjType tpe, int hunCount) {
		tbMap &d = getMap(tpe, hunCount);
		tbMap::iterator ite = d.find(key);
		if (ite == d.end()) {
			return false;
		}
		return true;
	}
	bool is3N2(int *list, int len) {
		int sum = accumulate(list, list + len, 0);
		if (sum % 3 != 2 || sum > 14) {
			return false;
		}
		return true;
	}
	void splitJiang(int *list, int len, int *arrJiang) {
		int j = 0;
		for (int i = 0; i < len; ++i) {
			if (list[i] >= 2 && j < 7) {
				arrJiang[j] = i;    // save index i
				j++;
			}
		}
	}
	void splitJiang(int *first, int *last, int *arrJiang) {
		int j = 0;
		int i = 0;
		while (first != last) {
			if (*first >= 2 && j < 7) {
				arrJiang[j] = i;
				++j;
			}
			++i;
			++first;
		}
	}
	bool checkFengSe(int *list, int len, int hunCount) {
		int iteFeng, iteWan, iteTiao, iteTong;
		iteFeng = iteWan = iteTiao = iteTong = 0;
		int feng[7], wan[9], tiao[9], tong[9];
		memset(feng, 0, sizeof(feng));
		memset(wan, 0, sizeof(wan));
		memset(tiao, 0, sizeof(tiao));
		memset(tong, 0, sizeof(tong));
		for (int i = 0; i < len; ++i) {
			if (list[i] < 0 || list[i] > 4) {
				return false;
			}
			if (list[i] == 0) {
				continue;
			}
			if (i >= 1 && i <= 7) {
				feng[iteFeng] = list[i];
				iteFeng++;
			}
			if (i >= 8 && i <= 16) {
				wan[iteWan] = list[i];
				iteWan++;
			}
			if (i >= 17 && i <= 25) {
				tiao[iteTiao] = list[i];
				iteTiao++;
			}
			if (i >= 26 && i <= 34) {
				tong[iteTong] = list[i];
				iteTong++;
			}
		}

		if (iteFeng > 0) {
			Key fKey = getKey(feng, sizeof(feng) / sizeof(feng[0]));
			if (!check(fKey, FengZi, hunCount)) {
				return false;
			}
		}
		if (iteWan > 0) {
			Key wKey = getKey(wan, sizeof(wan) / sizeof(wan[0]));
			if (!check(wKey, SanSe, hunCount)) {
				return false;
			}
		}
		if (iteTiao > 0) {
			Key tiKey = getKey(tiao, sizeof(tiao) / sizeof(tiao[0]));
			if (!check(tiKey, SanSe, hunCount)) {
				return false;
			}
		}
		if (iteTong > 0) {
			Key tKey = getKey(tong, sizeof(tong) / sizeof(tong[0]));
			if (!check(tKey, SanSe, hunCount)) {
				return false;
			}
		}

		return true;
	}
	bool check(int *list, int len) {
		// check 3n2
		if (!is3N2(list, len)) {
			return false;
		}

		// 过滤将牌
		int arrJiang[7]; // 最多有7个将
		memset(arrJiang, 0, sizeof(arrJiang));
		splitJiang(list, list + len, arrJiang);
		if (list[arrJiang[0]] < 2) {
			// 如果满足3n2,arrJiang一定会被赋值
			// 3n2 double check
			return false;
		}

		// todo
		int hunCount = 0;
		// 检查去将后的牌
		for (int i = 0; i < sizeof(arrJiang) / sizeof(arrJiang[0]); ++i) {
			list[arrJiang[i]] -= 2; // 过滤将
			if (checkFengSe(list, len, hunCount)) {
				return true;
			}
			list[arrJiang[i]] += 2;
		}
		return false;
	}
private:
	tbMap m_dFinalSanSeHun[MAX_HUN_COUNT+1];    // 最终存放带混三色数据
	tbMap m_dFinalFengHun[MAX_HUN_COUNT+1];     // 最终存放带混风字数据

	// tmpData
	tbVec m_dSanSeJiang;
	tbVec m_dSanSeKe;       // 用于生成三色牌Ke c(n/m)
	tbVec m_dSanSeShun;     // 用于生成三色牌Shun c(n/m)
	tbVec m_dFengJiang;
	tbVec m_dFengKe;        // 用于生成风牌Ke c(n/m)
};

void print_cards(int* cards) {
	printf("%d ", cards[0]);
	for (int i = 1; i <= 7; ++i) {
		printf("%d,", cards[i]);
	}
	printf(" ");

	for (int i = 8; i <= 16; ++i) {
		printf("%d,", cards[i]);
	}
	printf(" ");

	for (int i = 17; i <= 25; ++i) {
		printf("%d,", cards[i]);
	}
	printf(" ");

	for (int i = 26; i <= 34; ++i) {
		printf("%d,", cards[i]);
	}
	printf(" ");

	for (int i = 35; i <= 42; ++i) {
		printf("%d,", cards[i]);
	}

	printf("\n");
}
void benchmark() {
#define MAX_MJ_CODEARRAY 43
	MjMapTbV2 tb;
	tb.load();
	int MAX_COUNT = 100 * 10000;
	int GUI_NUM = 1;
	int source[MAX_COUNT * 9 * MAX_MJ_CODEARRAY];
	int allCards[144];
	int code = 0;
	for (int i = 0; i < 34; i++) {
		allCards[i * 4] = code + 1;
		allCards[i * 4 + 1] = code + 1;
		allCards[i * 4 + 2] = code + 1;
		allCards[i * 4 + 3] = code + 1;
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
	cout << "====shuffle begin" << endl;

	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
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
	timeUse = end.tv_sec - start.tv_sec;
	cout << "====shuffle end cost:" << timeUse << "s" << endl;

	cout << "====begin check..." << endl;
	// hu check
	gettimeofday(&start, NULL);
	int hu = 0;
	for (int n = 0; n < total; ++n) {
		//sleep(1);
		//print_cards(source+n*43);
		hu += tb.check(source + n * 43, 43);
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	cout << "check " << MAX_COUNT << " hu:" << hu << " cost:" << timeUse << "ms" << endl;
	cout << "====check end" << endl;
}

int main(int argc, char **argv) {
	MjMapTbV2 t;
	t.load();
	// 原手牌格式
	int cards[] = {
			0,                            // 混个数
			2, 3, 0, 0, 0, 0, 0,          // 风字牌1-7
			0, 0, 1, 1, 1, 0, 0, 0, 0,    // 万8-16
			0, 0, 0, 0, 0, 1, 1, 1, 0,    // 条17-25
			0, 0, 0, 0, 1, 1, 1, 0, 0,    // 筒26-34
			0, 0, 0, 0, 0, 0, 0, 0        // 花35-42
	};

	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	bool hu = t.check(cards, sizeof(cards) / sizeof(cards[0]));
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec - start.tv_usec);
	cout << "check hu?:" << hu << " cost:" << timeUse << "us" << endl;

	if (argc < 2) {
		cout << "USAGE:\n"
				"1,gen map to file.\n"
				"2,benchmark." << endl;
		return 0;
	}

	int tpe = atoi(argv[1]);
	if (tpe == 1) {
		// gen map
		MjMapTbV2 mt;
		mt.load();
		int tmp;
		cin >> tmp;
	}
	else if (tpe == 2) {
		// benchmark
		benchmark();
		//int tmp;
		//cin >> tmp;
	}
	else {
		cout << "arg err check usage." << endl;
		cout << "1,gen map to file.\n"
				"2,benchmark." << endl;
	}

	return 0;
}
