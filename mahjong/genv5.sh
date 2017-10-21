#!/bin/bash
if [ ! -d "tbl" ]; then
	mkdir tbl
fi
g++ mahjongHu_v5.cpp -g -ov5
