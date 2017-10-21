#!/bin/bash
if [ ! -d "tbl" ]; then
	mkdir tbl
fi
g++ genTbl.cpp set_table.cpp table_mgr.cpp hulib.cpp -g -otest
