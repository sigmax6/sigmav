#include <stdio.h>
#include <string.h>
#define MAX_IN 100
#define MAX_SEARCH 20

int main (int argc, char const* argv[])
{
    char inputStr[MAX_IN];
    char searchStr[MAX_SEARCH];
    int  i;
    int  counter = 0;
    char * inputPtr, * searchPtr;
    /*int  inputLen,searchLen,searchPtr;*/
    /*scanf("%s%s",inputStr,searchStr);*/
    gets(inputStr);
    gets(searchStr);

    /*searchLen = strlen(searchStr);*/
    /*inputLen = strlen(inputStr) - searchLen;*/

    for (inputPtr = inputStr ; * inputPtr != '\0' ; inputPtr++){
        for(searchPtr = searchStr,i = 0 ; * searchPtr != '\0' ; searchPtr++,i++){
            if(* searchPtr == '?'
               ||* searchPtr == * (inputStr+i) )
                continue;
            else
                break;
        }
        if(searchPtr == searchLen)
            counter++;
    }

    printf("%d\n",counter);

    return 0;

}

