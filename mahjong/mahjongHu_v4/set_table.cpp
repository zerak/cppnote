#include <stdio.h>
#include <string.h>
#include "set_table.h"
#include <algorithm>
#include <vector>

using namespace std;

bool compare(int a, int b) {
	return a < b;
}

SetTable::SetTable() {
}

SetTable::~SetTable() {
}

bool SetTable::check(int number) {
	return m_tbl.find(number) != m_tbl.end();
}

void SetTable::add(int key) {
    m_tbl.insert(key);
}

void SetTable::dump(char* name) {
    FILE *fp = fopen(name, "wb+");
    if(!fp) {
        return;
    }

    vector<int> vec;
    copy(m_tbl.begin(), m_tbl.end(), back_inserter(vec));
    sort(vec.begin(), vec.end(), compare);
    for(std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it) {
        fprintf(fp, "%d\n", *it);
    }
//    for(std::set<int>::iterator it=m_tbl.begin(); it!=m_tbl.end(); ++it) {
//        fprintf(fp, "%d\n", *it);
//    }

    fclose(fp);
}

void SetTable::load(char* name) {
    FILE *fp = fopen(name, "rb");
    if(!fp) {
        return;
    }

	int key = 0;
	int num = 0;
	while (fscanf(fp, "%d\n", &key) != EOF) {
		num++;
		add(key);
	}
    //printf("load %s %d key, set_size = %d\n", name, num, m_tbl.size());
    fclose(fp);
}
