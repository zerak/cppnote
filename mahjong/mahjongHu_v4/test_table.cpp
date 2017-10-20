#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <stdio.h>
#include "table_mgr.h"
#include "hulib.h"

using namespace std;

void print_cards(char* cards)
{
	for (int i = 0; i<9; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");

	for (int i = 9; i<18; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 18; i<27; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("  ");
	for (int i = 27; i<34; ++i)
	{
		printf("%d,", cards[i]);
	}
	printf("\n");
}

#define MAX_COUNT (10 * 10000)
static char s_HuCardAll[136];

void test_hu()
{
	char cards[] = {
		1,0,1,0,0,2,0,2,0,
		1,1,1,0,0,3,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,2
	};
	int hu = 0;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for (int i = 0; i < MAX_COUNT; i++){
		hu += HuLib::get_hu_info(cards, 34, 33);
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	cout << "total count:" << MAX_COUNT / 10000 << "wan hu:" << hu << " time:" << timeUse << "ms" << endl;
}

int GUI_NUM = 4;
char source[MAX_COUNT * 9 * 34];

int main()
{
	TableMgr::init();
	TableMgr::load();

	//test_hu();
	
	for (int i = 0; i < 34; i++)
	{
		s_HuCardAll[i * 4] = i;
		s_HuCardAll[i * 4 + 1] = i;
		s_HuCardAll[i * 4 + 2] = i;
		s_HuCardAll[i * 4 + 3] = i;
	}
	
	int gui_index = 33;
	int total = 0;
	srand(1);
	for (int n = 0; n < MAX_COUNT; ++n)
	{
		random_shuffle(s_HuCardAll, s_HuCardAll + 130);
		for (int i = 0; i < 9; ++i)	// 136/14 -> 9
		{
			char* cards = &source[total++ * 34];
			memset(cards, 0, 34);
			for (int j = i * 14; j < i * 14 + 14 - GUI_NUM; j++)
				++cards[s_HuCardAll[j]];
			cards[gui_index] = GUI_NUM;
		}
	}

	cout << "===begin benchmark" << endl;
	int hu = 0;
	double timeUse = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for (int n = 0; n<total; ++n)
	{
		char* cards = &source[n * 34];
		hu += HuLib::get_hu_info(cards, 34, gui_index);
	}
	gettimeofday(&end, NULL);
	timeUse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	cout << "total count:" << 9 * MAX_COUNT / 10000 << "wan hu:" << hu << " time:" << timeUse << "ms" << endl;
	return 0;
}
