#!/bin/bash
if [ ! -d "tbl" ]; then
	mkdir tbl
fi
g++ gen_table.cpp set_table.cpp table_mgr.cpp hulib.cpp -ogentb
