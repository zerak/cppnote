#include "table_mgr.h"
#include "hulib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>	// memset
#include <unistd.h> // sleep

using namespace std;

#define MAX_COUNT (100 * 10000)
static char s_HuCardAll[136];
char source[MAX_COUNT * 9 * 34];

void print_cards(char* cards) {
	for (int i = 0; i<9; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 9; i<18; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 18; i<27; ++i) {
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 27; i<34; ++i) {
		printf("%d,", cards[i]);
	}
	printf("\n");
}

void test_hu() {
	char cards[] = {
		1,0,1,0,0,2,0,2,0,
		1,1,1,0,0,3,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0
	};
	int hu = 0;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for (int i = 0; i < 1; i++){
		hu += HuLib::get_hu_info(cards, 2);
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 * 1000 + (end.tv_usec - start.tv_usec);
	cout << "check hu:" << hu << " time:" << timeUse << "us" << endl;
}

int main(int argc, char **argv) {
	TableMgr::init();
	TableMgr::load();

//	test_hu();

	int guiNum = 4;
	if(argc >= 2) {
		guiNum = atoi(argv[1]);
	}
	cout << "guiNum:" << guiNum << endl;
	
	for (int i = 0; i < 34; i++) {
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4 + 1] = i;
		s_HuCardAll[i * 4 + 2] = i;
		s_HuCardAll[i * 4 + 3] = i;
	}

	cout << "begin shuffle cards..." << endl;
	int total = 0;
	srand(1);
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for (int n = 0; n < MAX_COUNT; ++n) {
		random_shuffle(s_HuCardAll, s_HuCardAll + 130);
		for (int i = 0; i < 9; ++i)	{ // 136/14 -> 9
			char* cards = &source[total++ * 34];
			memset(cards, 0, 34);
			for (int j = i * 14; j < i * 14 + 14 - guiNum; j++)
				++cards[s_HuCardAll[j]];
		}
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec);
	cout << "shuffle cards end cost:" << timeUse << "s" << endl;

	cout << "===begin benchmark" << endl;
	int hu = 0;
	gettimeofday(&start, NULL);
	for (int n = 0; n<total; ++n) {
		char* cards = &source[n * 34];
		if (HuLib::get_hu_info(cards, guiNum)) {
			hu++;
			//sleep(1);
            //print_cards(cards);
		}
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	cout << "totalCount:" << 9 * MAX_COUNT / 10000 << "wan hu:" << hu << " guiNum:" << guiNum << " time:" << timeUse << "ms" << endl;
	cin >> hu;
	return 0;
}
