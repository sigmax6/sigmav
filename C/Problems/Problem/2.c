# include <stdio.h>

/**
 * ½« src ËùÖ¸µÄÇøÓòµÄÇ° n ¸ö×Ö½Ú¸´ÖÆµ½ dst ËùÖ¸µÄÇøÓò
 *
 * src ËùÖ¸ÇøÓòºÍ dst ËùÖ¸ÇøÓò¿ÉÒÔÖØµþ
 *
 * dst Ä¿±êµØÖ·
 * src À´Ô´µØÖ·
 * n ¸´ÖÆµÄÊýÁ¿
 *
 * ·µ»Ø 0
**/
int move(void * dst, void * src, size_t n)
{
    /* code in below area */
    char * dst_b=(char *)dst;
    char * src_b=(char *)src;
   // int dis=dst_b-src_b;
    int counter;
   // int counter_d=dis;
    //for (;counter_d<n;counter_d++)
   // {
   //     dst_b[counter_d]=src_b[counter_d];
   // }
   if(dst_b>src_b)
    for (counter=n-1;counter>=0;counter--)
    {
        //*(dst_b++)=*(src_b++);
        dst_b[counter]=src_b[counter];

    }
   else
    for (counter=0;counter<n;counter++)
    {
        dst_b[counter]=src_b[counter];
    }

    /* code in above area */
}

/* ²âÊÔº¯Êý */
int main()
{
    char buffer[16];
    char * src, * dst;
    int i;

    /* ½« 0 1 2 3 4 5 6 7 8 9 A B C D E F Ð´ÈëÊý×é */
    for(i = 0; i < 16; ++ i)
    {
        buffer[i] = i;
    }

    /* src Ö¸Ïò 2 */
    src = buffer + 2;
    /* dst Ö¸Ïò 6 */
    dst = buffer + 6;

    /* ÒÆ¶¯ÄÚ´æÇøÓò */
    move(dst, src, 8);

    /* Êä³ö½á¹û£¬ÓÒ±ßÊÇµÍµØÖ·×Ö½Ú£¬×ó±ßÊÇ¸ßµØÖ·×Ö½Ú */
    for(i = 7; i >= 0; -- i)
    {
        printf("%X ", dst[i]);
    }

    printf("\n");
    /* Êä³öµÄ½á¹ûÊÇ 9 8 7 6 5 4 3 2 */

    return 0;
}
