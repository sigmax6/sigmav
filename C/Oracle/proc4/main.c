
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
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

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
5,0,0,0,0,0,27,52,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,
36,0,0,2,49,0,4,55,0,0,4,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
67,0,0,3,89,0,6,64,0,0,4,4,0,1,0,1,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
98,0,0,4,0,0,30,70,0,0,0,0,0,1,0,
};


#line 1 "main.pc"
//Code by Li Feixiang 20100724 ver 1.0
//Modified by Sigmax6 20100725 ver 1.1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqlca.h>

#define MAX 200

struct roominfo_struct
{
    char roomnumber[50];
    char roomtype[50];
    char roomprice[50];
    char roomstate[50];
}ris[MAX];

//struct room_struct
//{
//    int roomid;
//    char roomnum[30];
//    int typeid;
//    char roomstate[30];
//    char roomtype[30];
//    char roomprice[30];
//}rs[MAX];

/* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 29 "main.pc"

    /* VARCHAR username[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } username;
#line 30 "main.pc"

    /* VARCHAR password[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } password;
#line 31 "main.pc"

    /* VARCHAR server[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } server;
#line 32 "main.pc"

    int operate;
    int count;
   int roomid;
    char roomnum[30];
   int typeid;
    char roomstate[30];
    char roomtype[30];
    char roomprice[30];
/* EXEC SQL END DECLARE SECTION; */ 
#line 41 "main.pc"


int main()
{

	strcpy(username.arr,"scott");
	username.len=strlen(username.arr);
	strcpy(password.arr,"hotel000");
	password.len=strlen(password.arr);
    strcpy(server.arr,"hotelDB");
    server.len=strlen(server.arr);
    /* EXEC SQL CONNECT :username IDENTIFIED BY :password USING :server ; */ 
#line 52 "main.pc"

{
#line 52 "main.pc"
    struct sqlexd sqlstm;
#line 52 "main.pc"
    sqlstm.sqlvsn = 12;
#line 52 "main.pc"
    sqlstm.arrsiz = 4;
#line 52 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 52 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 52 "main.pc"
    sqlstm.iters = (unsigned int  )10;
#line 52 "main.pc"
    sqlstm.offset = (unsigned int  )5;
#line 52 "main.pc"
    sqlstm.cud = sqlcud0;
#line 52 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 52 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 52 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 52 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)&username;
#line 52 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )22;
#line 52 "main.pc"
    sqlstm.sqhsts[0] = (         int  )22;
#line 52 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 52 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 52 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 52 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)&password;
#line 52 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )22;
#line 52 "main.pc"
    sqlstm.sqhsts[1] = (         int  )22;
#line 52 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 52 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 52 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 52 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)&server;
#line 52 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )22;
#line 52 "main.pc"
    sqlstm.sqhsts[2] = (         int  )22;
#line 52 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 52 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 52 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 52 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 52 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 52 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 52 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 52 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 52 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 52 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 52 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 52 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 52 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 52 "main.pc"
    sqlstm.sqlcmax = (unsigned int )100;
#line 52 "main.pc"
    sqlstm.sqlcmin = (unsigned int )2;
#line 52 "main.pc"
    sqlstm.sqlcincr = (unsigned int )1;
#line 52 "main.pc"
    sqlstm.sqlctimeout = (unsigned int )0;
#line 52 "main.pc"
    sqlstm.sqlcnowait = (unsigned int )0;
#line 52 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 52 "main.pc"
}

#line 52 "main.pc"

    if(sqlca.sqlcode==0)
	printf("connect database successful \n");
    /* EXEC SQL SELECT * INTO :ris FROM ROOMINFO; */ 
#line 55 "main.pc"

{
#line 55 "main.pc"
    struct sqlexd sqlstm;
#line 55 "main.pc"
    sqlstm.sqlvsn = 12;
#line 55 "main.pc"
    sqlstm.arrsiz = 4;
#line 55 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 55 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 55 "main.pc"
    sqlstm.stmt = "select * into :s1 ,:s2 ,:s3 ,:s4   from ROOMINFO ";
#line 55 "main.pc"
    sqlstm.iters = (unsigned int  )200;
#line 55 "main.pc"
    sqlstm.offset = (unsigned int  )36;
#line 55 "main.pc"
    sqlstm.selerr = (unsigned short)1;
#line 55 "main.pc"
    sqlstm.cud = sqlcud0;
#line 55 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 55 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 55 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 55 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)ris->roomnumber;
#line 55 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )50;
#line 55 "main.pc"
    sqlstm.sqhsts[0] = (         int  )sizeof(struct roominfo_struct);
#line 55 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 55 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 55 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 55 "main.pc"
    sqlstm.sqharc[0] = (unsigned int   *)0;
#line 55 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)ris->roomtype;
#line 55 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )50;
#line 55 "main.pc"
    sqlstm.sqhsts[1] = (         int  )sizeof(struct roominfo_struct);
#line 55 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 55 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 55 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 55 "main.pc"
    sqlstm.sqharc[1] = (unsigned int   *)0;
#line 55 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)ris->roomprice;
#line 55 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )50;
#line 55 "main.pc"
    sqlstm.sqhsts[2] = (         int  )sizeof(struct roominfo_struct);
#line 55 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 55 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 55 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 55 "main.pc"
    sqlstm.sqharc[2] = (unsigned int   *)0;
#line 55 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)ris->roomstate;
#line 55 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )50;
#line 55 "main.pc"
    sqlstm.sqhsts[3] = (         int  )sizeof(struct roominfo_struct);
#line 55 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 55 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 55 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 55 "main.pc"
    sqlstm.sqharc[3] = (unsigned int   *)0;
#line 55 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 55 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 55 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 55 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 55 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 55 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 55 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 55 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 55 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 55 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 55 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 55 "main.pc"
}

#line 55 "main.pc"

    printf("%d\n",sqlca.sqlerrd[2]);
    for(count=0;count<sqlca.sqlerrd[2];count++)
       printf("%.10s,%.10s,%.10s,%.10s\n",ris[count].roomnumber,ris[count].roomtype,ris[count].roomprice,ris[count].roomstate);
   // EXEC SQL SELECT * INTO :rs FROM SCOTT.ROOM;
   // for (count=0;count<2;count++)
   //     printf("%d,%s,%d,%s",rs[count].roomid,rs[count].roomnum,rs[count].typeid,rs[count].state);
    printf("Input room Number\n");
    scanf("%s",roomnum);
    /* EXEC SQL EXECUTE
     BEGIN
      SCOTT.PROCED_ROOM_INFO(:roomnum,:roomtype,:roomprice,:roomstate);
     END;
    END-EXEC; */ 
#line 68 "main.pc"

{
#line 64 "main.pc"
    struct sqlexd sqlstm;
#line 64 "main.pc"
    sqlstm.sqlvsn = 12;
#line 64 "main.pc"
    sqlstm.arrsiz = 4;
#line 64 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 64 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 64 "main.pc"
    sqlstm.stmt = "begin SCOTT . PROCED_ROOM_INFO ( :roomnum , :roomtype , \
:roomprice , :roomstate ) ; END ;";
#line 64 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 64 "main.pc"
    sqlstm.offset = (unsigned int  )67;
#line 64 "main.pc"
    sqlstm.cud = sqlcud0;
#line 64 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 64 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 64 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 64 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)roomnum;
#line 64 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )30;
#line 64 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 64 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 64 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)roomtype;
#line 64 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )30;
#line 64 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 64 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 64 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)roomprice;
#line 64 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )30;
#line 64 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 64 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 64 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqhstv[3] = (         void  *)roomstate;
#line 64 "main.pc"
    sqlstm.sqhstl[3] = (unsigned int  )30;
#line 64 "main.pc"
    sqlstm.sqhsts[3] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqindv[3] = (         void  *)0;
#line 64 "main.pc"
    sqlstm.sqinds[3] = (         int  )0;
#line 64 "main.pc"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 64 "main.pc"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 64 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 64 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 64 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 64 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 64 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 64 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 64 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 64 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 64 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 64 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 64 "main.pc"
}

#line 68 "main.pc"

    printf("ROOMINFO:%s,%s,%s,%s\n",roomnum,roomtype,roomprice,roomstate);
    /* EXEC SQL COMMIT RELEASE; */ 
#line 70 "main.pc"

{
#line 70 "main.pc"
    struct sqlexd sqlstm;
#line 70 "main.pc"
    sqlstm.sqlvsn = 12;
#line 70 "main.pc"
    sqlstm.arrsiz = 4;
#line 70 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 70 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 70 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 70 "main.pc"
    sqlstm.offset = (unsigned int  )98;
#line 70 "main.pc"
    sqlstm.cud = sqlcud0;
#line 70 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 70 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 70 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 70 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 70 "main.pc"
}

#line 70 "main.pc"

}
