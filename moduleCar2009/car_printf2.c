/****************************************************************************/
/* Program for using printf and scanf with micom car, Ver. 2.1              */
/*       April 2006, Renesas Technology Micom Car Rally Executive Committee */
/****************************************************************************/

/*======================================*/
/* Include                              */
/*======================================*/
#include    <no_float.h>                /* Simplifies stdio, place at beginning */
/*
If the float and double data types are not used in printf and scanf statements,
it is possible to reduce the size of the MOT file by including no_float.h before including stdio.h.
Do not include no_float.h if the double data type is used.
It is possible to use no_float.h only with the Renesas integrated development environment.
*/
#include    <stdio.h>
#include    <machine.h>
#include    "h8_3048.h"

/*======================================*/
/* Symbol definitions                   */
/*======================================*/
#define         CHECK_PRINTFSCANF 1     /* To use printf and scanf:  1*/
#define         SEND_BUFF_SIZE  64      /* Transmit buffer size       */
#define         RECV_BUFF_SIZE  32      /* Receive buffer size        */

/*======================================*/
/* Global variable declarations         */
/*======================================*/
#if CHECK_PRINTFSCANF
/* Transmit buffer */
char            send_buff[SEND_BUFF_SIZE];
char            *send_w = send_buff;
char            *send_r = send_buff;
unsigned int    send_count = 0;

/* Receive buffer */
char            recv_buff[RECV_BUFF_SIZE];
char            *recv_w = recv_buff;
char            *recv_r = recv_buff;

/* Variable settings for using printf and scanf */
unsigned char   sml_buf[4];
FILE _iob[] = { { &sml_buf[2],0,&sml_buf[0],3,_IOREAD          ,0,0 },
                { &sml_buf[3],0,&sml_buf[3],1,_IOWRITE|_IOUNBUF,0,1 } };

volatile int    _errno;

#endif

#if CHECK_PRINTFSCANF
/************************************************************************/
/* Initialize SCI1                                                      */
/* Arguments: baud rate register setting value                          */
/* Return values: none                                                  */
/************************************************************************/
void init_sci1( int smr, int brr )
{
    int i;

    SCI1_SCR = 0x00;
    SCI1_SMR = smr;
    SCI1_BRR = brr;
    for( i=0; i<10000; i++ );
    SCI1_SCR = 0x30;                    /* Enable transmit/receive  */
    SCI1_SSR &= 0x80;                   /* Clear error flag         */
}

/************************************************************************/
/* Receive one character                                                */
/* Arguments: Address where received character is stored                */
/* Return values: -1: receive error, 0: no data received, 1: data received, character stored in *s */
/************************************************************************/
int get_sci( char *s )
{
    int i;

    if( SCI1_SSR & 0x38 ) {
        /* Receive error */
        SCI1_SSR &= 0xc7;
        return -1;
    } else if( SCI1_SSR & 0x40 ) {
        /* Data received */
        *s = SCI1_RDR;
        SCI1_SSR &= 0xbf;
        return 1;
    }
    /* No data received */
    return 0;
}
/************************************************************************/
/* Store in transmit buffer                                             */
/* Arguments: Stored character                                          */
/* Return values: none                                                  */
/* Note: If buffer is full, wait until space is available.              */
/************************************************************************/
void setSendBuff(char c)
{
    /* Wait for buffer to be empty */
    while( SEND_BUFF_SIZE == send_count );

    or_ccr( 0x80 );                     /* Disable interrupt        */

    *send_w++ = c;
    if( send_w >= send_buff+SEND_BUFF_SIZE ) send_w = send_buff;
    send_count++;

    and_ccr( 0x7f );                    /* Enable interrupt         */

    /* Enable transmit interrupt */
    SCI1_SCR |= 0x80;
}

/************************************************************************/
/* Transmit interrupt                                                   */
/* Arguments: none                                                      */
/* Return values: none                                                  */
/************************************************************************/
#pragma interrupt (intTXI1)
void intTXI1( void )
{
    /* Set transmit data in register */
    SCI1_TDR = *send_r++;
    if( send_r >= send_buff+SEND_BUFF_SIZE ) send_r = send_buff;

    /* Start transmit */
    SCI1_SSR &= 0x7f;

    /* If this is last data unit, disable next and subsequent interrupts */
    send_count--;
    if( !send_count ) SCI1_SCR &= 0x7f;
}

/************************************************************************/
/* Function called by printf                                            */
/* Cannot be called by user                                             */
/************************************************************************/
char *sbrk(size_t size)
{
    return (char *)-1;
}

/************************************************************************/
/* Function called by printf                                            */
/* Cannot be called by user                                             */
/************************************************************************/
int write(int fileno,char *buf,unsigned int cnt)
{
    int i;
    static int (*func)(const char *,...) = printf;

    if( !cnt ) return 0;

    if( *buf == '\n' )  {
        setSendBuff( '\r' );
    } else if( *buf == '\b' ) {
        setSendBuff( '\b' );
        setSendBuff( ' ' );
    }
    setSendBuff( *buf );
    return cnt;
}

/************************************************************************/
/* Function called by scanf                                             */
/* Cannot be called by user                                             */
/************************************************************************/
int read(int fileno,char *buf,unsigned int cnt)
{
    static int (*func)(const char *,...) = scanf;

    if( !cnt ) return 0;

    if( recv_r == recv_w ) {
        do {
            /* Receive standby */
            while( !(SCI1_SSR & 0x40) )
            SCI1_SSR &= 0xc0;
            *buf = SCI1_RDR;
            SCI1_SSR &= 0xbf;

            switch( *buf ) {
                case '\b':  /* Backspace */
                    /* Backspace invalid if nothing in buffer */
                    if( recv_r == recv_w ) continue;
                    /* Move back one if data in buffer */
                    recv_w--;
                    break;
                case '\r':  /* Enter key */
                    *recv_w++ = *buf = '\n';
                    *recv_w++ = '\r';
                    break;
                default:
                    if( recv_w >= recv_buff+RECV_BUFF_SIZE-2 ) continue;
                    *recv_w++ = *buf;
                    break;
            }
            /* Each back: returns input character */
            write( fileno, buf, cnt );
        } while( *buf != '\n' );
    }
    *buf = *recv_r++;
    if( recv_r == recv_w ) recv_r = recv_w = recv_buff;

    return 1;
}

#endif

/************************************************************************/
/* Initialize RAM area                                                  */
/* Arguments: none                                                      */
/* Return values: none                                                  */
/************************************************************************/
void INITSCT( void )
{
    int *s, *e, *r;

    r = __sectop("R");                  /* Start of section R (RAM) */
    s = __sectop("D");                  /* Start of section D (ROM) */
    e = __secend("D");                  /* End of section D (ROM)   */
    while(s < e) {
        *r++ = *s++;                    /* Copy from D to R         */
    }

    s = __sectop("B");                  /* Start of section B (RAM) */
    e = __secend("B");                  /* End of section B (ROM)   */
    while(s < e) {
        *s++ = 0x00;                    /* B © 0x00                */
    }
}

/************************************************************************/
/* end of file                                                          */
/************************************************************************/