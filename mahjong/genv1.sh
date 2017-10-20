#!/bin/bash
if [ ! -d "tbl" ]; then
    mkdir tbl
fi
g++ mahjongHu_v1.cpp -ov1
