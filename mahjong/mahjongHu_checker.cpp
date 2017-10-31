
#include <set>
#include <string>
#include <iostream>
#include <map>
#include <sys/time.h>
#include <stdio.h>		// file op
#include <cstdlib>		// srand
//#include <stdlib.h>		// max
#include <algorithm>
#include <iostream>
#include <string.h>
#include <sys/time.h>

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
typedef bool Val;
typedef map<Key,Val> tbMap;


class MjMapTbV6 {
private:
#define MAX_TOTAL_TYPE 34
#define MAX_VAL_NUM 9
#define MAX_KEY_NUM (MAX_VAL_NUM+1)			//9+赖子
#define MAX_NAI_NUM 4						//赖子最大个数
#define BIT_VAL_NUM 3						//一个值占的bit数
#define BIT_VAL_FLAG 0x07					//
	typedef unsigned char BYTE ;

	//麻将颜色（种类）定义
	enum enColorMJ {
		enColorMJ_WAN = 0,  //万
		enColorMJ_TONG,     //筒
		enColorMJ_TIAO,     //条
		enColorMJ_FenZi,    //风、字 牌
		enColorMJ_Max,
	};

private:
	int getKeyByIndex(BYTE byIndex[MAX_KEY_NUM], BYTE byNum = MAX_KEY_NUM) {
		int nKey = 0;
		for (int i = 0; i < byNum; ++i)
			nKey |= (int)(byIndex[i] & BIT_VAL_FLAG) << (BIT_VAL_NUM*i);
		return nKey;
	}
	int getArrayIndex(BYTE byIndex[MAX_VAL_NUM], BYTE byNum = MAX_VAL_NUM) {
		int nKey = 0;
		for (int i = 0; i < byNum; ++i) {
			if ((byIndex[i] & BIT_VAL_FLAG) > 3) byIndex[i] -= 3;
			nKey |= (int)(byIndex[i] & 0x03) << (2 * i);
		}
		return nKey;
	}
	int getArrayIndex(int llVal) {
		BYTE byIndex[MAX_VAL_NUM] = {};
		for (int i = 0; i < MAX_VAL_NUM; ++i)
			byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;

		return getArrayIndex(byIndex);
	}
	bool isValidKey(int llVal) {
		BYTE byIndex[MAX_KEY_NUM] = {};
		for (int i = 0; i < MAX_KEY_NUM; ++i)
			byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;

		if (byIndex[9] > MAX_NAI_NUM)	return false;
		int nNum = 0;
		for (int i = 0; i < MAX_KEY_NUM; ++i) {
			nNum += byIndex[i];
			if (byIndex[i] > 4 || nNum > 14)	//
				return false;
		}
		return nNum > 0;
	}
	BYTE getNumByKey(int llVal, BYTE byNum = MAX_KEY_NUM) {
		BYTE byIndex[MAX_KEY_NUM] = {};
		for (int i = 0; i < MAX_KEY_NUM; ++i)
			byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;

		BYTE nNum = 0;
		for (int i = 0; i < byNum; ++i)
			nNum += byIndex[i];
		return nNum;
	}
	void addMap(map<int, BYTE> mapTemp[], int llVal) {
		BYTE nNum = getNumByKey(llVal, MAX_VAL_NUM);
		BYTE byNum = (llVal >> (BIT_VAL_NUM * 9))&BIT_VAL_FLAG;
		int  val = (llVal & 0x7FFFFFF);
		map<int, BYTE>::iterator iter = mapTemp[nNum].find(val);
		if (iter != mapTemp[nNum].end()) {
			iter->second = min(byNum, iter->second);
		} else {
			mapTemp[nNum][val] = byNum;
		}
	}

	void TrainSingle() {
		BYTE byTemp[MAX_KEY_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 };
		m_dSingle.insert(getKeyByIndex(byTemp));
		m_dSingleFZ.insert(getKeyByIndex(byTemp));
		// 1.1 刻子
		for (int i = 0; i < MAX_VAL_NUM; ++i) {
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 3; ++n) {
				byTemp[i] = 3 - n;
				byTemp[9] = n;
				m_dSingle.insert(getKeyByIndex(byTemp));
				//风、字牌最多7张
				if (i < 7) {
					m_dSingleFZ.insert(getKeyByIndex(byTemp));
				}
			}
		}
		// 1.2 顺子 没赖子
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i) {
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;
			byTemp[i + 1] = 1;
			byTemp[i + 2] = 1;
			m_dSingle.insert(getKeyByIndex(byTemp));
		}
		// 1.3 顺子 1个赖子 (2个赖子时也就是刻子)
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i) {
			for (int n = 0; n < 3; ++n) {
				memset(byTemp, 0, MAX_KEY_NUM);
				byTemp[i] = 1;
				byTemp[i + 1] = 1;
				byTemp[i + 2] = 1;
				byTemp[i + n] = 0;
				byTemp[9] = 1;
				m_dSingle.insert(getKeyByIndex(byTemp));
			}
		}
		// 2.1 将牌
		memset(byTemp, 0, MAX_KEY_NUM);
		byTemp[9] = 2;
		m_dSingleJiang.insert(getKeyByIndex(byTemp));
		m_dSingleJiangFZ.insert(getKeyByIndex(byTemp));
		for (int i = 0; i < MAX_VAL_NUM; ++i) {
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 2; ++n) {
				byTemp[i] = 2 - n;
				byTemp[9] = n;
				m_dSingleJiang.insert(getKeyByIndex(byTemp));
				//风、字牌最多7张
				if (i < 7) {
					m_dSingleJiangFZ.insert(getKeyByIndex(byTemp));
				}
			}
		}
	};
	void TrainAllComb(const set<int> &setSingle, map<int, BYTE> mapOut[]) {
		int nAll = 0;
		int nSingle[100] = {};
		set<int>::iterator iter = setSingle.begin();
		for (; iter != setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;

		for (int i1 = 0; i1 < nAll; ++i1) {
			addMap(mapOut, nSingle[i1]);
			for (int i2 = i1; i2 < nAll; ++i2) {
				int nTemp = nSingle[i1] + nSingle[i2];
				if (!isValidKey(nTemp))
					continue;

				addMap(mapOut, nTemp);
				for (int i3 = i2; i3 < nAll; ++i3) {
					int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3];
					if (!isValidKey(nTemp))
						continue;
					addMap(mapOut, nTemp);
					for (int i4 = i3; i4 < nAll; ++i4) {
						int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4];
						if (!isValidKey(nTemp))
							continue;
						addMap(mapOut, nTemp);
					}
				}
			}
		}
	}
	void TrainAllComb_Jiang(const set<int> &setSingle, map<int, BYTE> mapOut[]) {
		int nAll = 0;
		int nSingle[100] = {};

		set<int>::iterator iter = setSingle.begin();
		for (; iter != setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;

		map<int, BYTE> mapTemp[15];
		for (int j = 0; j < 15; ++j)
			mapTemp[j] = mapOut[j];

		for (int i = 0; i < nAll; ++i) {
			for (int j = 0; j < 15; ++j) {
				addMap(mapOut, nSingle[i]);
				map<int, BYTE>::iterator iter_u = mapTemp[j].begin();
				for (; iter_u != mapTemp[j].end(); ++iter_u) {
					int nTemp = nSingle[i] + iter_u->first + (int(iter_u->second & BIT_VAL_FLAG) << 27);
					if (isValidKey(nTemp))
						addMap(mapOut, nTemp);
				}
			}
		}
	}

public:
	void TrainAll() {
		if (m_dSingle.empty()) {
			memset(m_dArray, 0, sizeof(m_dArray));
			memset(m_dArrayFZ, 0, sizeof(m_dArrayFZ));
			memset(m_dError, 0, sizeof(m_dError));

			double timeUse = 0;
			struct timeval start;
			struct timeval end;
			gettimeofday(&start, NULL);
			TrainSingle();
			TrainAllComb(m_dSingle, m_dHuAll);
			TrainAllComb(m_dSingleFZ, m_dHuAllFZ);
			TrainAllComb_Jiang(m_dSingleJiang, m_dHuAll);
			TrainAllComb_Jiang(m_dSingleJiangFZ, m_dHuAllFZ);

			int numAll = 0;
			for (int i = 0; i < 15; ++i) {
				numAll += m_dHuAll[i].size();
				numAll += m_dHuAllFZ[i].size();
			}
			gettimeofday(&end, NULL);
			timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
			cout << "train cost:" << timeUse << "ms numAll:" << numAll << endl;

			for (int i = 0; i < 15; ++i) {
				map<int, BYTE>::iterator iter = m_dHuAll[i].begin();
				for (; iter != m_dHuAll[i].end(); ++iter) {
					int nArrayIndex = getArrayIndex(iter->first);
					m_dArray[nArrayIndex] = max(int(m_dArray[nArrayIndex]), iter->second + 1);
				}
				iter = m_dHuAllFZ[i].begin();
				for (; iter != m_dHuAllFZ[i].end(); ++iter) {
					int nArrayIndex = getArrayIndex(iter->first);
					m_dArrayFZ[nArrayIndex] = max(int(m_dArrayFZ[nArrayIndex]), iter->second + 1);
				}
				m_dHuAllFZ[i].clear();
			}

			for (int n = 0; n < int(sizeof(m_dArray)); ++n) {
				int nNum = 0;
				BYTE byIndex[MAX_VAL_NUM] = {};
				for (int i = 0; i < MAX_VAL_NUM; ++i) {
					byIndex[i] = (n >> (2 * i)) & 0x03;
					nNum += byIndex[i];
				}
				if (nNum >= 15)
					continue;

				int nVal = getKeyByIndex(byIndex, MAX_VAL_NUM);
				map<int, BYTE>::iterator iter = m_dHuAll[nNum].find(nVal);
				if (iter == m_dHuAll[nNum].end())
					m_dError[n] = 1;
			}
			for (int i = 0; i < 15; ++i)
				m_dHuAll[i].clear();

			// just show info
			int nZero = 0, nZeroFZ = 0, nError = 0, nAll = 0;
			for (int i = 0; i < int(sizeof(m_dArray)); ++i) {
				int nNum = 0;
				BYTE byIndex[MAX_VAL_NUM] = {};
				for (int n = 0; n < MAX_VAL_NUM; ++n) {
					byIndex[n] = (i >> (2 * n)) & 0x03;
					nNum += byIndex[n];
				}
				if (nNum >= 15)
					continue;

				++nAll;
				if (m_dArray[i] == 0)
					++nZero;
				if (m_dArrayFZ[i] == 0)
					++nZeroFZ;
				if (m_dError[i] == 0)
					++nError;
			}
			cout << "nAll = " << nAll << endl;
			cout << "nZero = " << nAll - nZero << " / " << nZero << endl;
			cout << "nZeroFZ = " << nAll - nZeroFZ << " / " << nZeroFZ << endl;
			cout << "nError = " << nAll - nError << " / " << nError << endl;
		} else {
			cout << "already trained!" << endl;
		}
	}

	bool CheckCanHu(BYTE *handList, int hunIndex) {
		BYTE byCards[MAX_TOTAL_TYPE + 1];
		memcpy(byCards, handList, MAX_TOTAL_TYPE + 1);
		int nNaiZiNum = 0;
		if (hunIndex <= MAX_TOTAL_TYPE) {
			nNaiZiNum = byCards[hunIndex];
			byCards[hunIndex] = 0;
		}

		BYTE byJiangNum = 0;
		BYTE nNaiTry = 0;
		BYTE nCount = 0xFF;
		for (int cor = 0; cor < enColorMJ_Max; ++cor) {
			int nMax = (cor == enColorMJ_FenZi) ? 7 : 9;
			int nVal = 0, nNum = 0;
			BYTE byDelIndex = 0xFF, byTemp = 0;
			for (int i = 0; i < nMax; ++i) {
				byTemp = byCards[9 * cor + i];
				nNum += byTemp;
				if (byTemp > 3) {
					if (0xFF != nCount) {
						if (i < nCount) {
							byDelIndex = min(i, int(nCount));
						}
					}
					else {
						byDelIndex = i;
					}
					nVal |= (int)(byTemp - 3) << (2 * i);
				}
				else {
					nVal |= (int)(byTemp) << (2 * i);
				}
			}

			if (nNum == 0)
				continue;

			if (nVal >= sizeof(m_dError) / sizeof(m_dError[0]))
				return false;

			if (m_dError[nVal])
				return false;

			nNaiTry = (cor == enColorMJ_FenZi) ? m_dArrayFZ[nVal] - 1 : m_dArray[nVal] - 1;
			if (nNaiTry != 0xFF) {
				byJiangNum += ((nNum + nNaiTry) % 3 == 2);
			}

			if (nNaiTry == 0xFF || nNaiZiNum < nNaiTry || byJiangNum + nNaiTry > nNaiZiNum + 1) {
				if (byDelIndex != 0xFF && nCount != byDelIndex) {
					if (nCount != 0xFF) {
						byCards[9 * cor + nCount] += 2;
						--byJiangNum;
						if (nNaiTry != 0xFF) {
							byJiangNum -= ((nNum + nNaiTry) % 3 == 2);
						}
					}
					byCards[9 * cor + byDelIndex] -= 2;
					--cor;
					++byJiangNum;

					nCount = byDelIndex;
					continue;
				}
				return false;
			}
			nNaiZiNum -= nNaiTry;
		}
		return byJiangNum > 0 || nNaiZiNum >= 2;
	}

private:
	set<int>	m_dSingle;			//单个顺子+刻子		50个
	set<int>	m_dSingleFZ;		//单个顺子+刻子		22个
	set<int>	m_dSingleJiang;		//单个将			19个
	set<int>	m_dSingleJiangFZ;	//单个将			15个

	BYTE	m_dArray[262144];
	BYTE	m_dArrayFZ[262144];
	BYTE	m_dError[262144];

	map<int, BYTE>	m_dHuAll[15];
	map<int, BYTE>	m_dHuAllFZ[15];
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

void print_cards(unsigned char* cards) {
	printf("%d, ", cards[34]);
	for (int i = 0; i <= 8; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 9; i <= 17; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 18; i <= 26; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 27; i <= 33; ++i) {
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
	MjMapTbV6 v6;
	v6.TrainAll();
	cout << "tran all" << endl;

	cout << "begin load" << endl;
	loadOf(tb, "total");
	cout << "end load len:" << tb.size() << endl;

	int num = 0;
	unsigned char cards[36] = { 0 };
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
			} else if (i > 0 && i <= 7) {
				cards[i+26] = key[i] - '0';
			} else if ( i >= 8 ) {
				cards[i-8] = key[i] - '0';
			}
		}
		cards[34] = key[0] - '0';

		// check
		if (v6.CheckCanHu(cards, 34)) {
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

