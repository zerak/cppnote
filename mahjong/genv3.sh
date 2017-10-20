#!/bin/bash
if [ ! -d "tbl" ]; then
	mkdir tbl
fi
g++ mahjongHu_v3.cpp -ov3
