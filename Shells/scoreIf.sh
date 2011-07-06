#!/bin/bash
#scoreIf.sh 

echo -e "Please input the score:\c"

while read SCORE
do
    if [ "$SCORE" -ge 0 ] && [ "$SCORE" -lt 60 ]
    then
        echo "Failed!"
    elif [ "$SCORE" -ge 60 ] && [ "$SCORE" -lt 70 ]
    then
        echo "Passed!"
    elif [ "$SCORE" -ge 70 ] && [ "$SCORE" -lt 80 ]
    then
        echo "Medium!"
    elif [ "$SCORE" -ge 80 ] && [ "$SCORE" -lt 90 ]
    then 
        echo "Good!"
    elif [ "$SCORE" -ge 90 ] && [ "$SCORE" -le 100 ]
    then
        echo "Excellent!"
    else
        echo "Exit!"
    fi
    echo -e "Please input the score:\c"
done

