#!/bin/bash

echo -e "Please input a score :\c"

while read SCORE
do
    case "$SCORE" in 
        ?|[1-5]?)
            echo "Failed!";;
        6?)
            echo "Passed!";;
        7?)
            echo "Medium!";;
        8?)
            echo "Good!";;
        9?|100)
            echo "Excellent";;
        *)
            echo "Exit";;
    esac
    echo -e "Please input a score :\c"
done

