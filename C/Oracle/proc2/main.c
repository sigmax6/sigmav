
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
5,0,0,1,0,0,30,21,0,0,0,0,0,1,0,
20,0,0,0,0,0,27,34,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,10,0,0,
51,0,0,3,74,0,4,62,0,0,3,1,0,1,0,2,97,0,0,2,97,0,0,1,3,0,0,
};


#line 1 "main.pc"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqlca.h>

#define NAME_LEN 20
#define PASS_LEN 20
#define DBNA_LEN 20
extern sqlglm(char *,int *,int *);

void connect();
void sql();
void sql_error();
void main()
{
    /* EXEC SQL WHENEVER SQLERROR DO sql_error(); */ 
#line 17 "main.pc"

    /* EXEC SQL WHENEVER NOT FOUND DO sql_error(); */ 
#line 18 "main.pc"

    connect();
    sql();
    /* EXEC SQL COMMIT RELEASE; */ 
#line 21 "main.pc"

{
#line 21 "main.pc"
    struct sqlexd sqlstm;
#line 21 "main.pc"
    sqlstm.sqlvsn = 12;
#line 21 "main.pc"
    sqlstm.arrsiz = 0;
#line 21 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 21 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 21 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 21 "main.pc"
    sqlstm.offset = (unsigned int  )5;
#line 21 "main.pc"
    sqlstm.cud = sqlcud0;
#line 21 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 21 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 21 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 21 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 21 "main.pc"
    if (sqlca.sqlcode < 0) sql_error();
#line 21 "main.pc"
}

#line 21 "main.pc"

}

void connect()
{
   // VARCHAR 
    char username[NAME_LEN],password[PASS_LEN],database[DBNA_LEN];
    strncpy((char *)username,"scott",NAME_LEN);
   // username.len=5;
    strncpy((char *)password,"hotel000",PASS_LEN);
   // password.len=8;
    strncpy((char *)database,"hotelDB",DBNA_LEN);
   // database.len=7;
    /* EXEC SQL CONNECT :username IDENTIFIED BY :password USING :database; */ 
#line 34 "main.pc"

{
#line 34 "main.pc"
    struct sqlexd sqlstm;
#line 34 "main.pc"
    sqlstm.sqlvsn = 12;
#line 34 "main.pc"
    sqlstm.arrsiz = 4;
#line 34 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 34 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 34 "main.pc"
    sqlstm.iters = (unsigned int  )10;
#line 34 "main.pc"
    sqlstm.offset = (unsigned int  )20;
#line 34 "main.pc"
    sqlstm.cud = sqlcud0;
#line 34 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 34 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 34 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 34 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)username;
#line 34 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )20;
#line 34 "main.pc"
    sqlstm.sqhsts[0] = (         int  )20;
#line 34 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 34 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 34 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 34 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)password;
#line 34 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )20;
#line 34 "main.pc"
    sqlstm.sqhsts[1] = (         int  )20;
#line 34 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 34 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 34 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 34 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)database;
#line 34 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )20;
#line 34 "main.pc"
    sqlstm.sqhsts[2] = (         int  )20;
#line 34 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 34 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 34 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 34 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 34 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 34 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 34 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 34 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 34 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 34 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 34 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 34 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 34 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 34 "main.pc"
    sqlstm.sqlcmax = (unsigned int )100;
#line 34 "main.pc"
    sqlstm.sqlcmin = (unsigned int )2;
#line 34 "main.pc"
    sqlstm.sqlcincr = (unsigned int )1;
#line 34 "main.pc"
    sqlstm.sqlctimeout = (unsigned int )0;
#line 34 "main.pc"
    sqlstm.sqlcnowait = (unsigned int )0;
#line 34 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 34 "main.pc"
    if (sqlca.sqlcode < 0) sql_error();
#line 34 "main.pc"
}

#line 34 "main.pc"

//    EXEC SQL CONNECT 'scott' IDENTIFIED BY 'hotel000' USING 'hotelDB';// USING :database;
    if(sqlca.sqlcode==0)
        printf("连接成功!\n");
    else
    {
        printf("连接失败!\n");
    }
}

void sql_error()
{
    char msg[200];
    size_t buf_len,msg_len;
    buf_len =sizeof(msg);
    sqlglm(msg,&buf_len,&msg_len);
    if(msg_len>buf_len)
        msg_len=buf_len;
    printf("%.*s\n\n",msg_len,msg);
}

void sql()
{
    int typeId;
    char typeName[10],typePrice[20];
    printf("请输入类型ID：");
    scanf("%d",&typeId);
    fflush(stdin);
    /* EXEC SQL SELECT TYPE_NAME,TYPE_PRICE INTO :typeName,:typePrice FROM roomtype WHERE TYPE_ID=:typeId; */ 
#line 62 "main.pc"

{
#line 62 "main.pc"
    struct sqlexd sqlstm;
#line 62 "main.pc"
    sqlstm.sqlvsn = 12;
#line 62 "main.pc"
    sqlstm.arrsiz = 4;
#line 62 "main.pc"
    sqlstm.sqladtp = &sqladt;
#line 62 "main.pc"
    sqlstm.sqltdsp = &sqltds;
#line 62 "main.pc"
    sqlstm.stmt = "select TYPE_NAME ,TYPE_PRICE into :b0,:b1  from roomtype\
 where TYPE_ID=:b2";
#line 62 "main.pc"
    sqlstm.iters = (unsigned int  )1;
#line 62 "main.pc"
    sqlstm.offset = (unsigned int  )51;
#line 62 "main.pc"
    sqlstm.selerr = (unsigned short)1;
#line 62 "main.pc"
    sqlstm.cud = sqlcud0;
#line 62 "main.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 62 "main.pc"
    sqlstm.sqlety = (unsigned short)4352;
#line 62 "main.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 62 "main.pc"
    sqlstm.sqhstv[0] = (         void  *)typeName;
#line 62 "main.pc"
    sqlstm.sqhstl[0] = (unsigned int  )10;
#line 62 "main.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 62 "main.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 62 "main.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqhstv[1] = (         void  *)typePrice;
#line 62 "main.pc"
    sqlstm.sqhstl[1] = (unsigned int  )20;
#line 62 "main.pc"
    sqlstm.sqhsts[1] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 62 "main.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 62 "main.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqhstv[2] = (         void  *)&typeId;
#line 62 "main.pc"
    sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
#line 62 "main.pc"
    sqlstm.sqhsts[2] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqindv[2] = (         void  *)0;
#line 62 "main.pc"
    sqlstm.sqinds[2] = (         int  )0;
#line 62 "main.pc"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 62 "main.pc"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 62 "main.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 62 "main.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 62 "main.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 62 "main.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 62 "main.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 62 "main.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 62 "main.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 62 "main.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 62 "main.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 62 "main.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 62 "main.pc"
    if (sqlca.sqlcode == 1403) sql_error();
#line 62 "main.pc"
    if (sqlca.sqlcode < 0) sql_error();
#line 62 "main.pc"
}

#line 62 "main.pc"

    if(sqlca.sqlcode==0)
        printf("类型名：%s,价格:%s\n",typeName,typePrice);
}
