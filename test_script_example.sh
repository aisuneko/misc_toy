#!/bin/bash
CASES=$(wc -l < input.txt)
BIN=./build/check
for ((i=1; i <= $CASES; i++)); do
        INPUT=$(sed -n "${i}p" input.txt)
        ANS=$(sed -n "${i}p" ans.txt)
        #echo $INPUT
        #echo $ANS
        diff --color <($BIN $INPUT) <(echo -e $ANS)
        if [ $? -eq 0 ]; then
                echo "Test case $i/$CASES passed"
        else
                echo "Test case $i/$CASES FAILED"
                exit 1
        fi
done
