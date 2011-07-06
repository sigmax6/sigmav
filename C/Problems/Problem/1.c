/* 一次遍历将字符串"abcdefghijklmnopqrstuvwxyz"（已知26个）翻转 */
# include <stdio.h>
#define LEN 26
int main()
{
    char str[] = "abcdefghijklmnopqrstuvwxyz";

    /* code in below area */
    char temp;
    int ptr1=0;
    int ptr2=LEN-1;
    int midpos=(LEN)/2;
    while(ptr1!=midpos)
    {
        temp=str[ptr1];
        str[ptr1]=str[ptr2];
        str[ptr2]=temp;
        ptr1++;
        ptr2--;
    }
    /* code in above area */

    printf("%s\n", str);

    return 0;
}
