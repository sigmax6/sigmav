#include <stdio.h>

void * MyMemcpy(void * dest,const void * src,int n)
{

	char * destPtr=(char *)dest;
	char * srcPtr=(char *)src;

	for(int i=0;i<n;i++)
	{
		*(destPtr+i)=*(srcPtr+i);
	}

	return dest;

}

void * MyMemcpy2(void * dest,const void * src,int size)
{

	assert(dest != NULL);
	assert(src != NULL);

    if( (src<dest) && (((char *)src + size) > (char *)dest) )
    {

    	char * destPtr = (char *)dest + size - 1;
    	char * srcPtr = (char *)src + size - 1;

    	while(size--)
    	{
    		*(destPtr--) = *(srcPtr--);
    	}

    }
    else
    {

    	char * destPtr = (char *)dest;
    	char * srcPtr = (char *)src;

    	while(size--)
    	{
    		*(destPtr++) = *(srcPtr++);
    	}

    }

     return dest;

}

int main(int argc , const char *argv[])
{
     char * str1 = (char *)malloc(128);
     char * str2 = (char *)malloc(128);
     char * str3= str1 + 20;
     char * str4= str1 + 3;
     char * str5= str2 + 20;
     char * str6 = str2 + 3;

     strcpy(str1,"test string1");
     strcpy(str2,"test string2");

     printf("Str1:%s\n",str1);
     printf("Str2:%s\n",str2);

     MyMemcpy(str3,str1,strlen(str1)*sizeof(char));
     printf("After MyMemcpy(str3,str1,strlen(str1)*sizeof(char));\n Str3:%s",str3);
     MyMemcpy(str4,str1,strlen(str1)*sizeof(char));
     printf("After MyMemcpy(str4,str1,strlen(str1)*sizeof(char));\n Str4:%s",str4);
	 MyMemcpy2(str5,str2,strlen(str2)*sizeof(char));
     printf("After MyMemcpy2(str5,str2,strlen(str2)*sizeof(char));\n Str3:%s",str5);
     MyMemcpy2(str6,str2,strlen(str2)*sizeof(char));
     printf("After MyMemcpy2(str6,str2,strlen(str2)*sizeof(char));\n Str4:%s",str6);

     return 0;
}
