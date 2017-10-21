#include "table_mgr.h"
#include "set_table.h"
#include <stdio.h>
#include <stdlib.h>

#define TABLECLASS SetTable

Table* TableMgr::m_check_table[MAX_GUI_NUM + 1];
Table* TableMgr::m_check_eye_table[MAX_GUI_NUM + 1];
Table* TableMgr::m_check_feng_table[MAX_GUI_NUM + 1];
Table* TableMgr::m_check_feng_eye_table[MAX_GUI_NUM + 1];

void TableMgr::init() {
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
		m_check_table[i] = new TABLECLASS;
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
		m_check_eye_table[i] = new TABLECLASS;
    }
 
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
		m_check_feng_table[i] = new TABLECLASS;
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
		m_check_feng_eye_table[i] = new TABLECLASS;
    }
}

void TableMgr::destroy() {
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        delete m_check_table[i];
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        delete m_check_eye_table[i];
    }
 
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        delete m_check_feng_table[i];
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        delete m_check_feng_eye_table[i];
    }
}

bool TableMgr::check(int key, int gui_num, bool eye, bool sanSe) {
    Table* tbl = 0;
    if(sanSe) {
        if(eye) {
            tbl = m_check_eye_table[gui_num];
        } else {
            tbl = m_check_table[gui_num];
        }
    } else {
        if(eye) {
            tbl = m_check_feng_eye_table[gui_num];
        } else {
            tbl = m_check_feng_table[gui_num];
        }
    }

    if(tbl) {
        return tbl->check(key);
    }
    return false;
}

void TableMgr::add(int key, int gui_num, bool eye, bool chi) {
    Table* tbl = 0;
    if(chi) {
        if(eye) {
            tbl = m_check_eye_table[gui_num];
        } else {
            tbl = m_check_table[gui_num];
        }
    }
    else {
        if(eye) {
            tbl = m_check_feng_eye_table[gui_num];
        } else {
            tbl = m_check_feng_table[gui_num];
        }
    }

    if (tbl) {
        tbl->add(key);
    } else {
        printf("%d",gui_num);
    }
}

bool TableMgr::load() {
    char path[256];
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/table_%d.tbl", i);
        m_check_table[i]->load(path);
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/eye_table_%d.tbl", i);
        m_check_eye_table[i]->load(path);
    }
 
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/feng_table_%d.tbl", i);
        m_check_feng_table[i]->load(path);
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/feng_eye_table_%d.tbl", i);
        m_check_feng_eye_table[i]->load(path);
    }
    return true;
}

bool TableMgr::dump_table() {
    char path[256];
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/table_%d.tbl", i);
        m_check_table[i]->dump(path);
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/eye_table_%d.tbl", i);
        m_check_eye_table[i]->dump(path);
    }
    return true;
}

bool TableMgr::dump_feng_table() {
    char path[256];
	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/feng_table_%d.tbl", i);
        m_check_feng_table[i]->dump(path);
    }

	for (int i = 0; i<MAX_GUI_NUM + 1; ++i) {
        sprintf(path, "tbl/feng_eye_table_%d.tbl", i);
        m_check_feng_eye_table[i]->dump(path);
    }
    return true;
}

bool TableMgr::gen() {
    return true;
}
