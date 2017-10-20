
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
	void load() {
		if (!loadTb(SanSe, 0)) {
			cout << "gen map" << endl;
			//genSanSeJiang();
			genSanSeKe();       // 生成三色牌Ke
			genSanSeShun();     // 生成三色牌ShunZi
			//genFengJiang();
			genFengKe();        // 生成风字牌Ke

			genSanSeMap();
			genFengMap();
			return;
		}

		//cout << "sanseKe:" << m_dSanSeKe.size() << endl;
		//cout << "sanseShun:" << m_dSanSeShun.size() << endl;
		//cout << "fengKe:" << m_dFengKe.size() << endl;

		// begin from 1hun
		for (int i = 1; i < sizeof(m_dFinalSanSeHun) / sizeof(m_dFinalSanSeHun[0]); ++i) {
			loadTb(SanSe, i);
		}
		// load feng
		for (int j = 0; j < sizeof(m_dFinalFengHun) / sizeof(m_dFinalFengHun[0]); ++j) {
			loadTb(FengZi, j);
		}
	}

	// handList: 当前手牌
	// len: handList长度
	// mjCcodeIdx: 最后拿的牌index
	// hunCount: 混个数
	bool checkHu(int *handList, int len, int hunCount) {
		// check 3n2
//        if (!is3N2(handList, len, hunCount)) {
//            return false;
//        }

		if (hunCount > 0) {
			return checkHuHun(handList, len, hunCount);
		}

		// 过滤将牌
		int arrJiang[7]; // 最多有7个将
		memset(arrJiang, 0, sizeof(arrJiang));
		int jiangCount = splitJiang(handList, handList + len, arrJiang);
		if (handList[arrJiang[0]] < 2) {
			// 如果满足3n2,arrJiang一定会被赋值
			// 3n2 double check
			return false;
		}

		// todo
		// 检查去将后的牌
		for (int i = 0; i < jiangCount; ++i) {
			//cout << "check jiang:" << i << " " << arrJiang[i] << endl;
//            if(arrJiang[i] <= 0) {
//                return false;
//            }
			handList[arrJiang[i]] -= 2; // 过滤将
			if (splitFengSe(handList, len, hunCount)) {
				handList[arrJiang[i]] += 2;
				return true;
			}
			handList[arrJiang[i]] += 2;
		}
		return false;
	}

private:
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
//        cout << "key:" << key << " buf:" << buf << endl;
//        for (int j = 0; j < vec.size(); ++j) {
//            cout << vec[j] << " ";
//        }
//        cout << "vec size:" << vec.size() << endl;
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
	bool is3N2(int *list, int len, int hunCount) {
		int sum = accumulate(list, list + len, hunCount);
		if (sum % 3 != 2 || sum > 14) {
			return false;
		}
		return true;
	}
	int splitFengJiang(int *list, int len, int *arrJiang) {
		int j = 0;
		for (int i = 0; i < len; ++i) {
			if (list[i] == 2 && j < 7) {    // 风牌的将必须 == 2
				arrJiang[j] = i;    // save index i
				j++;
			}
		}
		return j;
	}
	int splitJiang(int *first, int *last, int *arrJiang) {
		int j = 0;
		int i = 0;
		while (first != last) {
			if (*first >= 2 && j < 7) {     // 三色牌将可以是 >= 2
				arrJiang[j] = i;
				++j;
			}
			++i;
			++first;
		}
		return j;
	}
	bool splitFengSe(int *list, int len, int hunCount) {
		int feng[7];
		memset(feng, 0, sizeof(feng));
		memcpy(feng, list+1, sizeof(feng));
		if (accumulate(feng, feng + 7, 0) > 0) {
			Key fKey = getKey(feng, 7);
			if (!check(fKey, FengZi, hunCount)) {
				return false;
			}
		}

		int wan[9];
		int size = sizeof(wan);
		memset(wan, 0, sizeof(wan));
		memcpy(wan, list+1+7, size);
		if (accumulate(wan, wan + 9, 0) > 0) {
			Key wKey = getKey(wan, 9);
			if (!check(wKey, SanSe, hunCount)) {
				return false;
			}
		}

		int tiao[9];
		memset(tiao, 0, sizeof(tiao));
		memcpy(tiao, list+1+7+9, size);
		if (accumulate(tiao, tiao + 9, 0) > 0) {
			Key tiKey = getKey(tiao, 9);
			if (!check(tiKey, SanSe, hunCount)) {
				return false;
			}
		}

		int tong[9];
		memset(tong, 0, sizeof(tong));
		memcpy(tong, list+1+7+9+9, size);
		if (accumulate(tong, tong + 9, 0) > 0) {
			Key tKey = getKey(tong, 9);
			if (!check(tKey, SanSe, hunCount)) {
				return false;
			}
		}

		return true;
	}
	bool checkSanSe(int *list, int len, int hunCount) {
		int wanCount, tiaoCount, tongCount;
		wanCount = tiaoCount = tongCount = 0;

		int wan[9];
		int size = sizeof(wan);
		memset(wan, 0, sizeof(wan));
		memcpy(wan, list+1+7, size);
		wanCount = accumulate(wan, wan + 9, 0);
		if (wanCount > 0) {
			int arrJiang[7];
			memset(arrJiang, 0, sizeof(arrJiang));
			int jiangCount = splitJiang(wan, wan + 7, arrJiang);
			if (jiangCount <= 0) {
				Key wKey = getKey(wan, 9);
				int i = 0;
				bool huFlag = false;
				for (i ; i < hunCount+1; ++i) {
					if (check(wKey, SanSe, i)) {
						huFlag = true;
						break;
					}
				}
				// 分配所有混,依然不能胡
				if (i == hunCount && !huFlag) {
					return false;
				}
			} else {
				if (wanCount - 2 > 0 ) {   // 当只有一个将时, arrJiang去掉两张牌为空,所以有此判断
					int i = 0;
					bool huFlag = false;
					wan[arrJiang[0]] -= 2;
					Key wKey = getKey(wan, 9);
					for (i ; i < hunCount+1; ++i) {
						if (check(wKey, SanSe, i)) {
							huFlag = true;
							break;
						}
					}
					wan[arrJiang[0]] += 2;
					// 分配所有混,依然不能胡
					if (i == hunCount && !huFlag) {
						return false;
					}
				}
			}
		}
		return false;
	}
	bool checkHuHun(int *list, int len, int hunCount) {
		int needHun = 0;
		int fengCount, wanCount, tiaoCount, tongCount;
		fengCount = wanCount = tiaoCount = tongCount = 0;

		int feng[7];
		memset(feng, 0, sizeof(feng));
		memcpy(feng, list+1, sizeof(feng));
		fengCount = accumulate(feng, feng + 7, 0);
		if (fengCount > 0) {
			int arrFengJiang[7];
			memset(arrFengJiang, 0, sizeof(arrFengJiang));
			int jiangCount = splitFengJiang(feng, 7, arrFengJiang);
			if (jiangCount <= 0) {  // todo
				/*风牌没有将,此时手牌风牌个数可能为1or3,如:(1,0,0,1), (1,1,0,0), (3,0,0,3),
                  不可能是2or4,(Note:手牌有4个风不能胡)*/
				Key fKey = getKey(feng, 7);
				int i = 0;
				bool huFlag = false;
				for (i ; i < hunCount+1; ++i) {
					if (check(fKey, FengZi, i)) {
						needHun += i;
						huFlag = true;
						break;
					}
				}
				// 分配所有混,依然不能胡
				if (i > hunCount && !huFlag) {
					return false;
				}
			} else { //(jiangCount >= 1) // 手牌可能为 1,2,2 || 2,0,2,1 || 2
				if (fengCount - 2 > 0 ) {   // 当只有一个将时, arrFengJiang去掉两张牌为空,所以有此判断
					int i = 0;
					bool huFlag = false;
					feng[arrFengJiang[0]] -= 2;
					Key fKey = getKey(feng, 7);
					for (i ; i < hunCount+1; ++i) {
						if (check(fKey, FengZi, i)) {
							needHun += i;
							huFlag = true;
							break;
						}
					}
					feng[arrFengJiang[0]] += 2;
					// 分配所有混,依然不能胡
					if (i > hunCount && !huFlag) {
						return false;
					}
				}
			}
		}

		int wan[9];
		memset(wan, 0, sizeof(wan));
		memcpy(wan, list+1+7, sizeof(wan));
		wanCount = accumulate(wan, wan + 9, 0);
		if (wanCount > 0) {
			int arrJiang[7];
			memset(arrJiang, 0, sizeof(arrJiang));
			int jiangCount = splitJiang(wan, wan + 7, arrJiang);
			if (jiangCount <= 0) {  // 0,0,1, 0,0,0, 0,0,1 todo
				Key wKey = getKey(wan, 9);
				int i = 0;
				bool huFlag = false;
				for (i ; i < hunCount+1; ++i) {
					if (check(wKey, SanSe, i)) {
						needHun += i;
						huFlag = true;
						break;
					}
				}
				// 分配所有混,依然不能胡
				if (i > hunCount && !huFlag) {
					return false;
				}
			} else {
				if (wanCount - 2 > 0 ) {   // 当只有一个将时, arrJiang去掉两张牌为空,所以有此判断
					int i = 0;
					bool huFlag = false;
					wan[arrJiang[0]] -= 2;
					Key wKey = getKey(wan, 9);
					for (i ; i < hunCount+1; ++i) {
						if (check(wKey, SanSe, i)) {
							needHun += i;
							huFlag = true;
							break;
						}
					}
					wan[arrJiang[0]] += 2;
					// 分配所有混,依然不能胡
					if (i > hunCount && !huFlag) {
						return false;
					}
				}
			}
		}

		int tiao[9];
		memset(tiao, 0, sizeof(tiao));
		memcpy(tiao, list+1+7+9, sizeof(tiao));
		tiaoCount = accumulate(tiao, tiao+ 9, 0);
		if (tiaoCount > 0) {
			int arrJiang[7];
			memset(arrJiang, 0, sizeof(arrJiang));
			int jiangCount = splitJiang(tiao, tiao + 7, arrJiang);
			if (jiangCount <= 0) {
				Key tKey = getKey(tiao, 9);
				int i = 0;
				bool huFlag = false;
				for (i ; i < hunCount+1; ++i) {
					if (check(tKey, SanSe, i)) {
						needHun += i;
						huFlag = true;
						break;
					}
				}
				// 分配所有混,依然不能胡
				if (i > hunCount && !huFlag) {
					return false;
				}
			} else {
				if (tiaoCount - 2 > 0 ) {   // 当只有一个将时, arrJiang去掉两张牌为空,所以有此判断
					int i = 0;
					bool huFlag = false;
					tiao[arrJiang[0]] -= 2;
					Key tKey = getKey(tiao, 9);
					for (i ; i < hunCount+1; ++i) {
						if (check(tKey, SanSe, i)) {
							needHun += i;
							huFlag = true;
							break;
						}
					}
					tiao[arrJiang[0]] += 2;
					// 分配所有混,依然不能胡
					if (i > hunCount && !huFlag) {
						return false;
					}
				}
			}
		}

		int tong[9];
		memset(tong, 0, sizeof(tong));
		memcpy(tong, list+1+7+9+9, sizeof(tong));
		tongCount = accumulate(tong, tong+ 9, 0);
		if (tongCount> 0) {
			int arrJiang[7];
			memset(arrJiang, 0, sizeof(arrJiang));
			int jiangCount = splitJiang(tong, tong + 7, arrJiang);
			if (jiangCount <= 0) {
				Key tKey = getKey(tong, 9);
				int i = 0;
				bool huFlag = false;
				for (i ; i < hunCount+1; ++i) {
					if (check(tKey, SanSe, i)) {
						needHun += i;
						huFlag = true;
						break;
					}
				}
				// 分配所有混,依然不能胡
				if (i > hunCount && !huFlag) {
					return false;
				}
			} else {
				if (tongCount - 2 > 0 ) {   // 当只有一个将时, arrJiang去掉两张牌为空,所以有此判断
					int i = 0;
					bool huFlag = false;
					tong[arrJiang[0]] -= 2;
					Key tKey = getKey(tong, 9);
					for (i ; i < hunCount+1; ++i) {
						if (check(tKey, SanSe, i)) {
							needHun += i;
							huFlag = true;
							break;
						}
					}
					tong[arrJiang[0]] += 2;
					// 分配所有混,依然不能胡
					if (i > hunCount && !huFlag) {
						return false;
					}
				}
			}
		}

		if ( (fengCount + wanCount + tiaoCount + tongCount + hunCount) % 3 != 2 ) {
			return false;
		}

		if (needHun <= hunCount) {
			return true;
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
void benchmark(MjMapTbV2& tb) {
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
		if (tb.checkHu(source + n * MAX_MJ_CODEARRAY, MAX_MJ_CODEARRAY, hunCount)) {
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
void check(MjMapTbV2& t) {
	cout << "====begin checkV1 ..." << endl;
	// 原手牌格式
	int cards[] = {
			0,                            // 混个数
			1, 0, 0, 0, 0, 0, 1,          // 风字牌1-7
			0, 0, 0, 0, 0, 0, 0, 0, 0,    // 万8-16
			0, 0, 0, 0, 0, 0, 3, 0, 0,    // 条17-25
			0, 0, 0, 0, 0, 0, 0, 0, 0,    // 筒26-34
			0, 0, 0, 0, 0, 0, 0, 0        // 花35-42
	};

	int hunCount = 4;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	bool hu = t.checkHu(cards, sizeof(cards) / sizeof(cards[0]), hunCount);
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec - start.tv_usec);
	cout << "====checkV1 hu?:" << hu << " cost:" << timeUse << "us" << endl;
}
void checkV2(MjMapTbV2 &tb) {
	cout << "====begin checkV2 ..." << endl;
	vector<int> vec;
	int cards[] = {
//            0, 0,0,0,0,0,0,0,  0,0,0,1,1,1,0,0,0,  0,1,1,1,1,1,1,0,0,  0,1,1,1,0,0,0,0,0,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,  1,1,1,3,0,1,1,1,0,  0,0,0,0,1,1,1,0,0,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  1,2,2,1,0,0,0,0,0,  0,0,1,1,1,0,1,1,1,  0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,0,1,1,1,  0,0,0,0,1,1,1,0,0,  3,0,0,0,0,1,1,1,0,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,1,2,2,1,0,0,0,0,  0,0,0,0,0,0,1,1,1,  2,0,0,0,0,0,1,1,1,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,1,0,0,0,0,0,0,1,  0,0,1,0,0,0,0,1,0,  0,0,0,0,0,0,0,1,1,  0,0,0,0,0,0,0,0,
			0, 0,0,0,0,0,0,0,  1,0,0,0,0,0,0,1,0,  0,0,1,0,0,0,0,1,0,  0,0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0,  // 8混测试
//            0, 0,0,0,0,0,0,0,  0,0,0,0,3,0,1,1,1,  1,1,1,0,0,0,0,0,0,  0,0,0,0,0,0,1,1,1,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,  0,0,0,0,0,1,1,1,0,  3,1,1,1,0,0,1,1,1,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,  3,0,1,2,2,1,0,0,0,  0,0,0,1,1,1,0,0,0,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,0,1,1,1,  1,1,1,0,0,0,0,0,0,  0,0,1,1,1,0,1,1,1,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  1,1,1,0,0,0,0,0,0,  0,1,1,1,0,1,1,1,0,  0,0,0,0,0,0,1,1,1,  0,0,0,0,0,0,0,0,
//            0, 0,0,0,0,0,0,0,  0,0,0,0,0,1,1,1,0,  0,0,0,1,1,1,0,0,0,  0,1,1,1,0,1,1,1,0,  0,0,0,0,0,0,0,0,
	};
	int hu = 0;
	int hunCount = 8;
	for (int i = 0; i < sizeof(cards) / sizeof(cards[0]) / 43; ++i) {
		if (tb.checkHu(cards + i*43, 35, hunCount)) {
			print_cards(cards + i *43);
			++hu;
		}
	}
	cout << "====checkV2 hu:" << hu << endl;
}
void checkV3(MjMapTbV2 &t) {
	cout << "====begin checkV3 ..." << endl;
	char name[256];
	sprintf(name, "tbl/table_%s.tbl", "test");
	FILE *fp = fopen(name, "rb");
	if (fp == NULL) {
		cout << "can't load table:" << name << endl;
		return;
	}

	map<string, bool> data;
	char str[MAX_MJ_CODEARRAY];
	memset(str, 0, sizeof(str));
	int num = 0;
	while (fscanf(fp, "%s\n", str) != EOF) {
		string key = str;
		data.insert(make_pair(key, true));
		num++;
		if(num >= 100*10000) {
			break;
		}
	}
	fclose(fp);

	int hu = 0;
	int hunCount = 4;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);

	map<string, bool>::iterator ite = data.begin();
	for(ite; ite != data.end(); ++ite) {
		int list[43];
		memset(list, 0, sizeof(list));
		string key = ite->first;
		for (int i = 0; i < key.length(); ++i) {
			string tmp;
			tmp = key[i];
			list[i] = atoi(tmp.c_str());
		}
		if(t.checkHu(list, 43, hunCount)) {
			++hu;
		} else {
			//print_cards(list);
		}
		//int num = atoi(key.c_str());
		//cout << "key:" << key << " num:" << num << endl;
	}
	cout << "Hu:" << hu << " mapSize:" << data.size() << endl;
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000;
	cout << "checkV3 hu?:" << hu << " cost:" << timeUse << "ms" << endl;
}
int main(int argc, char **argv) {
	MjMapTbV2 t;
	t.load();
	check(t);
//    checkV2(t);
//    checkV3(t);

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
		benchmark(t);
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
