#include<stdio.h>
#include<ctype.h>

#define BUFFSIZE 64
#define IRSIZE 16
#define IRNUM  18
#define RNUM 8
#define SHIFTNUM 4
char * IRS[IRNUM]=
{
    "nop",
    "ldr",
    "str",
    "mov",
    "jpb",
    "jpz",
    "jmp",
    "add",
    "addc",
    "adc",
    "adcc",
    "sub",
    "subc",
    "sbc",
    "sbcc",
    "and",
    "or",
    "not"
};
char * r[RNUM]=
{
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7"
};
char * shift[SHIFTNUM]=
{
    "lsl",
    "lsr",
    "rsl",
    "rsr"
};
//------------------------------------------
int checkBlank(char * * ptr)
{
    while(* * ptr!='\0'&&isspace(**ptr)) //(*ptr==' '||*ptr=='\t'||*ptr=='\n'))
    {
        (*ptr)++;
    }
    if(**ptr=='\0')
    {
        return 1;
    }
    return 0;
}
//------------------------------------------
int checkBegin(char * ir_buff)
{
    char * begin="begin";
    char * ptr=ir_buff;
    int counter=0;
    while(*(ptr++)!='.') ;
    for (counter=0;counter<5;counter++,ptr++)
    {
        if(*ptr!=begin[counter]&&*ptr!=begin[counter]-32)
        {
            return 0;
        }
    }
    return checkBlank(&ptr);
}
//-------------------------------------------
int checkEnd(char * ir_buff)
{
    char * end="end";
    char * ptr=ir_buff;
    int counter=0;
    while(*(ptr++)!='.') ;
    for (counter=0;counter<3;counter++,ptr++)
    {
        if(*ptr!=end[counter]&&*ptr!=end[counter]-32)
        {
            return 0;
        }
    }
    return checkBlank(&ptr);
}
//----------------------------------------------
int decode(char * ir_buff,char * ir_decoder,char * errinfo)
{
    char * ptr=ir_buff;
    char * idf[8]={NULL};
    int arg_count=0;
    int i=0,j=0;
    int find=0;
    if(checkBlank(&ptr))
    {
       return 0;
    }
    else
    {
       while(*ptr!='\0')
       {
           idf[arg_count++]=ptr;
           while(!isspace(*ptr))
           {
               ptr++;
           }
           while(isspace(*ptr))
           {
               *(ptr++)='\0';
           }
       }

       i=0;
           printf("%d,%sx\n",i,idf[i]);
           for(j=0;j<IRNUM;j++)
           {
              if(!strcmp(idf[i],IRS[j]))
              {
                  find=1;
                  break;
              }
           }
           if(find!=1)
           {
              strcpy(errinfo,"No match IR found!");
              return -1;
           }
           else
           {
              switch(j)
              {
              case 0:
                  strcpy(ir_decoder,"0000000000000000");
                  break;
              case 1:
                  for(j=0;j<RNUM;j++)
                  {
                      if(!strcmp(idf))
                  }
              }
           }
       }
       putchar('\n');
       return 1;
    }
}
//----------------------------------------------------------
int main(int argc,char * argv[])
{
    FILE * ifp;                   //Input file pointer;
    FILE * ofp;                   //Output file pointer;
    char ir_buff[BUFFSIZE];       //IR buffer
    char ir_decoder[IRSIZE];      //IR decoder
    char errorinfo[BUFFSIZE];     //Error Info
    int  line_num=0;              //Line Number
    int  begin_end=0;             //0 NOT BEGIN 1 BEGINING -1 ENDED
    if(argc<=1)
    {
        printf("No input file!\n");
        return -1;
    }
    else if(argc>1)
    {
        if((ifp=fopen(argv[1],"r"))==NULL)
        {
            printf("Open File %s Error!\n",argv[1]);
            fclose(ifp);
            return -1;
        }
        else
        {
          if((ofp=fopen("out.t","w"))==NULL)
          {
              printf("Can not open output file!\n");
              fclose(ifp);
              fclose(ofp);
              return -1;
          }
          while(fgets(ir_buff,BUFFSIZE,ifp)!=NULL)
          {
              line_num++;
              if((begin_end==0)&&checkBegin(ir_buff))
              {
                  begin_end=1;
                  printf("Begins\n");
              }
              else if(begin_end==1)
              {
                  if(checkEnd(ir_buff))
                  {
                    begin_end=-1;
                    printf("Ends\n");
                    break;
                  }
                  else
                  {
                      int t=0;
                      printf("%d:%s\n",line_num,ir_buff);
                      if((t=decode(ir_buff,ir_decoder,errorinfo))==-1)
                      {
                           printf("%d:%s\n",line_num,errorinfo);
                      }
                  }
              }
          }
          if(begin_end==0)
          {
              printf("No Codes!\n");
          }
          else if (begin_end==1)
          {
              printf("No End error!\n");
          }
          fclose(ofp);
          fclose(ifp);
        }
    }
    else
    {
        return -1;
    }
}
