#!/bin/bash
DATA=$(date +'%F'_'%s')
LOG=log/log_$1_$DATA.txt

./proj.sh $1 >& "$LOG"