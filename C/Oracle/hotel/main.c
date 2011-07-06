
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[8];
};
static struct sqlcxp sqlfpn =
{
    7,
    "main.pc"
};


static unsigned int sqlctx = 9211;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[6];
   unsigned int   sqhstl[6];
            int   sqhsts[6];
            void  *sqindv[6];
            int   sqinds[6];
   unsigned int   sqharm[6];
   unsigned int   *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {12,6};

/* SQLLIB Prototypes */
extern sqlcxt ( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlcx2t( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlbuft( void **, char * );
extern sqlgs2t( void **, char * );
extern sqlorat( void **, unsigned int *, void * );

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem( unsigned char *, signed int * );

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4130,873,0,0,
5,0,0,0,0,0,27,61,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,
36,0,0,2,0,0,30,117,0,0,0,0,0,1,0,
51,0,0,3,55,0,4,124,0,0,4,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
82,0,0,4,89,0,6,137,0,0,4,4,0,1,0,1,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
113,0,0,5,83,0,6,151,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,3,0,0,
144,0,0,6,61,0,6,164,0,0,2,2,0,1,0,1,3,0,0,2,3,0,0,
167,0,0,7,54,0,6,171,0,0,1,1,0,1,0,1,3,0,0,
186,0,0,8,108,0,4,185,0,0,6,1,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,3,0,0,2,9,0,0,
1,3,0,0,
225,0,0,9,113,0,6,218,0,0,6,6,0,1,0,1,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,3,0,0,
1,9,0,0,
};


#line 1 "main.pc"
//Code by Li Feixiang 20100724 ver 1.0
//Modified by Sigmax6 20100725 ver 1.1
//Modified by Sigmax6 20100726 ver 1.2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqlca.h>

#define MAX 200                  //The MAX number of rooms

/* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 13 "main.pc"
  //DECLAER SECTION--------------------------
struct roominfo_struct
{
    char roomnumber[50];
    char roomtype[50];
    char roomprice[50];
    char roomstate[50];
}ris[MAX];

    /* VARCHAR username[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } username;
#line 22 "main.pc"

    /* VARCHAR password[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } password;
#line 23 "main.pc"

    /* VARCHAR server[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } server;
#line 24 "main.pc"

    int count;
    int operate;
    char roomnum[30];
    char roomstate[30];
    char roomtype[30];
    char roomprice[30];

    char guestname[20];
    char guestsex[10];
    char mobile[11];
    int roomid;
    int guestid;
    /* VARCHAR arrivetime[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } arrivetime;
#line 37 "main.pc"

/* EXEC SQL END DECLARE SECTION; */ 
#line 38 "main.pc"
   //END DECLAER SECTION ----------------------

//Functions to use
void getRoomInfo();
void getRoomDetile();
void checkIn();
void checkOut();
void checkGuestInfo();
void sqlError();

int main()
{
    //REG　THE ERROR HANDLE
    /* EXEC SQL WHENEVER SQLERROR DO sqlError(); */ 
#line 51 "main.pc"

    /* EXEC SQL WHENEVER NOT FOUND DO sqlError(); */ 
#line 52 "main.pc"


    //CONNECT---------------------------------
    strcpy(username.arr,"scott");
    username.len=strlen(username.arr);
    strcpy(password.arr,"hotel000");
    password.len=strlen(password.arr);
    strcpy(server.arr,"hotelDB");
    server.len=strlen(server.arr);
    /* EXEC SQL CONNECT :username IDENTIFIED BY :password USING :server ; */ 
#line 61 "main.pc"

{
#line 61 "main.pc"
    struct sqlexd sqlstm;
#line 61 "main.pc"
    sqlstm.sqlvsn = 12;
#line 61 "main.pc"
    sqlstm.arrsiz = 4;
#line 61 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 61 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 61 "main.pc"
    sqlstm.iters = (unsigned int  )10;
#line 61 "main.pc"
    sqlstm.offset = (unsigned int  )5;
#line 61 "main.pc"
    sqlstm.cud = sqlcud0;
#line 61 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 61 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 61 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 61 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)&username;
#line 61 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )22;
#line 61 "main.pc"
    sqlstm.sqhsts[0] = (         int  )22;
#line 61 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 61 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 61 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 61 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)&password;
#line 61 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )22;
#line 61 "main.pc"
    sqlstm.sqhsts[1] = (         int  )22;
#line 61 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 61 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 61 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 61 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)&server;
#line 61 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )22;
#line 61 "main.pc"
    sqlstm.sqhsts[2] = (         int  )22;
#line 61 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 61 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 61 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 61 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 61 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 61 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 61 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 61 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 61 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 61 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 61 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 61 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 61 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 61 "main.pc"
    sqlstm.sqlcmax = (unsigned int )100;
#line 61 "main.pc"
    sqlstm.sqlcmin = (unsigned int )2;
#line 61 "main.pc"
    sqlstm.sqlcincr = (unsigned int )1;
#line 61 "main.pc"
    sqlstm.sqlctimeout = (unsigned int )0;
#line 61 "main.pc"
    sqlstm.sqlcnowait = (unsigned int )0;
#line 61 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 61 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 61 "main.pc"
}

#line 61 "main.pc"

    system("clear");
    if(sqlca.sqlcode==0)
    {
        printf("连接数据库成功！\n");
    }
    else
    {
        printf("连接失败，程序退出\n");
        exit(1);
    }
    //END CONNECT -----------------------------

    //MAIN FUNC OF THE SYSTEM------------------
    
    printf("=================SIGMA 酒店管理系统 0708==================\n\n");
    while(1)
    {
	printf("****************请选择功能*****************\n");
	printf("                                         \n");
	printf("              1.查看酒店状态                \n");
	printf("              2.查看房间信息                \n");
	printf("              3.客户入住                   \n");
	printf("              4.客户退房                   \n");
	printf("              5.客户信息管理                \n");
	printf("              6.退出管理系统                \n");
    printf("                                         \n");
	printf("*****************************************\n");
	printf("请输入操作指令：");
	scanf("%d",&operate);
        switch(operate)
		{
			case 1:
				getRoomInfo();
				break;
			case 2:
				getRoomDetile();
				break;
			case 3:
				checkIn();
				break;
			case 4:
				checkOut();
				break;
			case 5:
                checkGuestInfo();
				break;
            case 6:
				break;
			default:
				printf("选择错误，请选择1－6操作！\n");
				break;
		}
	if(operate==6)
	    break;
     }
     /* EXEC SQL COMMIT RELEASE; */ 
#line 117 "main.pc"

{
#line 117 "main.pc"
     struct sqlexd sqlstm;
#line 117 "main.pc"
     sqlstm.sqlvsn = 12;
#line 117 "main.pc"
     sqlstm.arrsiz = 4;
#line 117 "main.pc"
     sqlstm.sqladtp = &sqladt;
#line 117 "main.pc"
     sqlstm.sqltdsp = &sqltds;
#line 117 "main.pc"
     sqlstm.iters = (unsigned int  )1;
#line 117 "main.pc"
     sqlstm.offset = (unsigned int  )36;
#line 117 "main.pc"
     sqlstm.cud = sqlcud0;
#line 117 "main.pc"
     sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 117 "main.pc"
     sqlstm.sqlety = (unsigned short)4352;
#line 117 "main.pc"
     sqlstm.occurs = (unsigned int  )0;
#line 117 "main.pc"
     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 117 "main.pc"
     if (sqlca.sqlcode < 0) sqlError();
#line 117 "main.pc"
}

#line 117 "main.pc"

     printf("退出登录！\n");   
}

void getRoomInfo()
{
    system("clear");
    /* EXEC SQL SELECT * INTO :ris FROM SCOTT.ROOMINFO; */ 
#line 124 "main.pc"

{
#line 124 "main.pc"
    struct sqlexd sqlstm;
#line 124 "main.pc"
    sqlstm.sqlvsn = 12;
#line 124 "main.pc"
    sqlstm.arrsiz = 4;
#line 124 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 124 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 124 "main.pc"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4   from SCOTT.ROOMINFO ";
#line 124 "main.pc"
    sqlstm.iters = (unsigned int  )200;
#line 124 "main.pc"
    sqlstm.offset = (unsigned int  )51;
#line 124 "main.pc"
    sqlstm.selerr = (unsigned short)1;
#line 124 "main.pc"
    sqlstm.cud = sqlcud0;
#line 124 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 124 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 124 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 124 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)ris->roomnumber;
#line 124 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )50;
#line 124 "main.pc"
    sqlstm.sqhsts[0] = (         int  )sizeof(struct roominfo_struct);
#line 124 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 124 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 124 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 124 "main.pc"
    sqlstm.sqharc[0] = (unsigned int   *)0;
#line 124 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)ris->roomtype;
#line 124 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )50;
#line 124 "main.pc"
    sqlstm.sqhsts[1] = (         int  )sizeof(struct roominfo_struct);
#line 124 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 124 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 124 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 124 "main.pc"
    sqlstm.sqharc[1] = (unsigned int   *)0;
#line 124 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)ris->roomprice;
#line 124 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )50;
#line 124 "main.pc"
    sqlstm.sqhsts[2] = (         int  )sizeof(struct roominfo_struct);
#line 124 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 124 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 124 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 124 "main.pc"
    sqlstm.sqharc[2] = (unsigned int   *)0;
#line 124 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)ris->roomstate;
#line 124 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )50;
#line 124 "main.pc"
    sqlstm.sqhsts[3] = (         int  )sizeof(struct roominfo_struct);
#line 124 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 124 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 124 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 124 "main.pc"
    sqlstm.sqharc[3] = (unsigned int   *)0;
#line 124 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 124 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 124 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 124 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 124 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 124 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 124 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 124 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 124 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 124 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 124 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 124 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 124 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 124 "main.pc"
}

#line 124 "main.pc"

    printf("房间号       ｜ 房间类型          ｜房间价格        ｜房间状态\n");
    for(count=0;count<sqlca.sqlerrd[2];count++)
    {
       printf("%.10s,%.10s,%.10s,%.10s\n",ris[count].roomnumber,ris[count].roomtype,ris[count].roomprice,ris[count].roomstate);
    }
}

void getRoomDetile()
{
    printf("请输入要查询的房间号:");
    scanf("%s",roomnum);
    system("clear");
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_ROOM_INFO(:roomnum,:roomtype,:roomprice,:roomstate);
     END;
    END-EXEC; */ 
#line 141 "main.pc"

{
#line 137 "main.pc"
    struct sqlexd sqlstm;
#line 137 "main.pc"
    sqlstm.sqlvsn = 12;
#line 137 "main.pc"
    sqlstm.arrsiz = 4;
#line 137 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 137 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 137 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_ROOM_INFO ( :roomnum , :roomtype , \
:roomprice , :roomstate ) ; END ;";
#line 137 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 137 "main.pc"
    sqlstm.offset = (unsigned int  )82;
#line 137 "main.pc"
    sqlstm.cud = sqlcud0;
#line 137 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 137 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 137 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 137 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)roomnum;
#line 137 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )30;
#line 137 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 137 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 137 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)roomtype;
#line 137 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )30;
#line 137 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 137 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 137 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)roomprice;
#line 137 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )30;
#line 137 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 137 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 137 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)roomstate;
#line 137 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )30;
#line 137 "main.pc"
    sqlstm.sqhsts[3] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 137 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 137 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 137 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 137 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 137 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 137 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 137 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 137 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 137 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 137 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 137 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 137 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 137 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 137 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 137 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 137 "main.pc"
}

#line 141 "main.pc"

    printf("房间信息:房间号       ｜ 房间类型          ｜房间价格        ｜房间状态\n");
    printf("%s,%s,%s,%s\n",roomnum,roomtype,roomprice,roomstate);
}

void checkIn()
{
    printf("请依次输入登记的用户名、性别、电话、房间号(之间用空格逗号分开))\n");
    scanf("%s,%s,%s,%d",guestname,guestsex,mobile,&roomid);
    system("clear");
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_CHECKIN(:guestname,:guestsex,:mobile,:roomid);
     END;
    END-EXEC; */ 
#line 155 "main.pc"

{
#line 151 "main.pc"
    struct sqlexd sqlstm;
#line 151 "main.pc"
    sqlstm.sqlvsn = 12;
#line 151 "main.pc"
    sqlstm.arrsiz = 4;
#line 151 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 151 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 151 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_CHECKIN ( :guestname , :guestsex , \
:mobile , :roomid ) ; END ;";
#line 151 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 151 "main.pc"
    sqlstm.offset = (unsigned int  )113;
#line 151 "main.pc"
    sqlstm.cud = sqlcud0;
#line 151 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 151 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 151 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 151 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)guestname;
#line 151 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )20;
#line 151 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 151 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 151 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)guestsex;
#line 151 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )10;
#line 151 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 151 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 151 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)mobile;
#line 151 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )11;
#line 151 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 151 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 151 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)&roomid;
#line 151 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
#line 151 "main.pc"
    sqlstm.sqhsts[3] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 151 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 151 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 151 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 151 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 151 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 151 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 151 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 151 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 151 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 151 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 151 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 151 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 151 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 151 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 151 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 151 "main.pc"
}

#line 155 "main.pc"

    printf("用户登记成功！\n");
}

void checkOut()
{
    int account;
    printf("请输入要结帐的用户的编号：");
    scanf("%d",&guestid);
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_CHECKOUT(:guestid,:account);
     END;
    END-EXEC; */ 
#line 168 "main.pc"

{
#line 164 "main.pc"
    struct sqlexd sqlstm;
#line 164 "main.pc"
    sqlstm.sqlvsn = 12;
#line 164 "main.pc"
    sqlstm.arrsiz = 4;
#line 164 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 164 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 164 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_CHECKOUT ( :guestid , :account ) ; \
END ;";
#line 164 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 164 "main.pc"
    sqlstm.offset = (unsigned int  )144;
#line 164 "main.pc"
    sqlstm.cud = sqlcud0;
#line 164 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 164 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 164 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 164 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)&guestid;
#line 164 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
#line 164 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 164 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 164 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 164 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 164 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 164 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 164 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)&account;
#line 164 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
#line 164 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 164 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 164 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 164 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 164 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 164 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 164 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 164 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 164 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 164 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 164 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 164 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 164 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 164 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 164 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 164 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 164 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 164 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 164 "main.pc"
}

#line 168 "main.pc"

    printf("住房总金额为：%d",account);
    printf("正在删除用户信息.......");
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_DELETE_GUEST(:guestid);
     END;
    END-EXEC; */ 
#line 175 "main.pc"

{
#line 171 "main.pc"
    struct sqlexd sqlstm;
#line 171 "main.pc"
    sqlstm.sqlvsn = 12;
#line 171 "main.pc"
    sqlstm.arrsiz = 4;
#line 171 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 171 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 171 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_DELETE_GUEST ( :guestid ) ; END ;";
#line 171 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 171 "main.pc"
    sqlstm.offset = (unsigned int  )167;
#line 171 "main.pc"
    sqlstm.cud = sqlcud0;
#line 171 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 171 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 171 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 171 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)&guestid;
#line 171 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
#line 171 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 171 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 171 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 171 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 171 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 171 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 171 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 171 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 171 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 171 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 171 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 171 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 171 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 171 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 171 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 171 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 171 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 171 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 171 "main.pc"
}

#line 175 "main.pc"

    printf("删除成功！");
    return;
}

void checkGuestInfo()
{
    char ch='n';
    printf("请输入要更新客户的编号：");
    scanf("%d",&guestid);
    /* EXEC SQL SELECT GUEST_NAME,SEX,MOBILE,ROOM_ID,ARRIVE_TIME INTO :guestname,:guestsex,:mobile,:roomid,:arrivetime FROM GUEST WHERE GUEST_ID=:guestid; */ 
#line 185 "main.pc"

{
#line 185 "main.pc"
    struct sqlexd sqlstm;
#line 185 "main.pc"
    sqlstm.sqlvsn = 12;
#line 185 "main.pc"
    sqlstm.arrsiz = 6;
#line 185 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 185 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 185 "main.pc"
    sqlstm.stmt = "select GUEST_NAME ,SEX ,MOBILE ,ROOM_ID ,ARRIVE_TIME int\
o :b0,:b1,:b2,:b3,:b4  from GUEST where GUEST_ID=:b5";
#line 185 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 185 "main.pc"
    sqlstm.offset = (unsigned int  )186;
#line 185 "main.pc"
    sqlstm.selerr = (unsigned short)1;
#line 185 "main.pc"
    sqlstm.cud = sqlcud0;
#line 185 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 185 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 185 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)guestname;
#line 185 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )20;
#line 185 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)guestsex;
#line 185 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )10;
#line 185 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)mobile;
#line 185 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )11;
#line 185 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)&roomid;
#line 185 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
#line 185 "main.pc"
    sqlstm.sqhsts[3] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqhstv[4] = (         void  *)&arrivetime;
#line 185 "main.pc"
    sqlstm.sqhstl[4] = (unsigned int  )22;
#line 185 "main.pc"
    sqlstm.sqhsts[4] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[4] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[4] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[4] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqhstv[5] = (         void  *)&guestid;
#line 185 "main.pc"
    sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
#line 185 "main.pc"
    sqlstm.sqhsts[5] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqindv[5] = (         void  *)0;
#line 185 "main.pc"
    sqlstm.sqinds[5] = (         int  )0;
#line 185 "main.pc"
    sqlstm.sqharm[5] = (unsigned int  )0;
#line 185 "main.pc"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 185 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 185 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 185 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 185 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 185 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 185 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 185 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 185 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 185 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 185 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 185 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 185 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 185 "main.pc"
}

#line 185 "main.pc"

    printf("更新客户名吗？y/n");
    scanf("%c",&ch);
    if(ch=='y')
    {
        printf("请输入新的客户名：");
        scanf("%s",guestname);
        ch='n';
    }
    printf("更新客户性别吗？y/n");
    scanf("%c",&ch);
    if(ch=='y')
    {
        printf("请输入新的客户性别：");
        scanf("%s",guestsex);
        ch='n';
    }
    printf("更新客户联系方式吗？y/n");
    scanf("%c",&ch);
    if(ch=='y')
    {
        printf("请输入新的客户联系方式：");
        scanf("%s",mobile);
        ch='n';
    }
    printf("更改客户房间号？y/n");
    scanf("%c",&ch);
    if(ch=='y')
    {
        printf("请输入新的客户房间号：");
        scanf("%d",&roomid);
        ch='n';
    }
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_MODIFY_GUEST(:guestid,:guestname,:guestsex,:mobile,:roomid,:arrivetime);
     END;
    END-EXEC; */ 
#line 222 "main.pc"

{
#line 218 "main.pc"
    struct sqlexd sqlstm;
#line 218 "main.pc"
    sqlstm.sqlvsn = 12;
#line 218 "main.pc"
    sqlstm.arrsiz = 6;
#line 218 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 218 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 218 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_MODIFY_GUEST ( :guestid , :guestnam\
e , :guestsex , :mobile , :roomid , :arrivetime ) ; END ;";
#line 218 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 218 "main.pc"
    sqlstm.offset = (unsigned int  )225;
#line 218 "main.pc"
    sqlstm.cud = sqlcud0;
#line 218 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 218 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 218 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)&guestid;
#line 218 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
#line 218 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)guestname;
#line 218 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )20;
#line 218 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)guestsex;
#line 218 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )10;
#line 218 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)mobile;
#line 218 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )11;
#line 218 "main.pc"
    sqlstm.sqhsts[3] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqhstv[4] = (         void  *)&roomid;
#line 218 "main.pc"
    sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
#line 218 "main.pc"
    sqlstm.sqhsts[4] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[4] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[4] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[4] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqhstv[5] = (         void  *)&arrivetime;
#line 218 "main.pc"
    sqlstm.sqhstl[5] = (unsigned int  )22;
#line 218 "main.pc"
    sqlstm.sqhsts[5] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqindv[5] = (         void  *)0;
#line 218 "main.pc"
    sqlstm.sqinds[5] = (         int  )0;
#line 218 "main.pc"
    sqlstm.sqharm[5] = (unsigned int  )0;
#line 218 "main.pc"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 218 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 218 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 218 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 218 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 218 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 218 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 218 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 218 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 218 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 218 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 218 "main.pc"
    if (sqlca.sqlcode == 1403) sqlError();
#line 218 "main.pc"
    if (sqlca.sqlcode < 0) sqlError();
#line 218 "main.pc"
}

#line 222 "main.pc"

    printf("信息更新成功！");
    return;
}

void sqlError()
{
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 
#line 229 "main.pc"

    printf("错误!\n");
    printf("%.70s\n",sqlca.sqlerrm.sqlerrmc);
}
