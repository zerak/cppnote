#!/bin/bash
if [ ! -d "tbl" ]; then
	mkdir tbl
fi
g++ test_table.cpp set_table.cpp table_mgr.cpp hulib.cpp -ov4
