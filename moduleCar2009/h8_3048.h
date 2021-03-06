/****************************************************************************************/
/* I/O register definitions for on-chip peripheral functions of H8/3048F-ONE, Ver. 1.03 */
/*   April 2006, Renesas Technology Micom Car Rally Executive Committee                 */
/****************************************************************************************/

#define P1DDR           (*(volatile unsigned char*)0xfffc0)
#define P1DR            (*(volatile unsigned char*)0xfffc2)
#define P2DDR           (*(volatile unsigned char*)0xfffc1)
#define P2DR            (*(volatile unsigned char*)0xfffc3)
#define P3DDR           (*(volatile unsigned char*)0xfffc4)
#define P3DR            (*(volatile unsigned char*)0xfffc6)
#define P4DDR           (*(volatile unsigned char*)0xfffc5)
#define P4DR            (*(volatile unsigned char*)0xfffc7)
#define P5DDR           (*(volatile unsigned char*)0xfffc8)
#define P5DR            (*(volatile unsigned char*)0xfffca)
#define P6DDR           (*(volatile unsigned char*)0xfffc9)
#define P6DR            (*(volatile unsigned char*)0xfffcb)
#define P7DR            (*(volatile unsigned char*)0xfffce)
#define P8DDR           (*(volatile unsigned char*)0xfffcd)
#define P8DR            (*(volatile unsigned char*)0xfffcf)
#define P9DDR           (*(volatile unsigned char*)0xfffd0)
#define P9DR            (*(volatile unsigned char*)0xfffd2)
#define PADDR           (*(volatile unsigned char*)0xfffd1)
#define PADR            (*(volatile unsigned char*)0xfffd3)
#define PBDDR           (*(volatile unsigned char*)0xfffd4)
#define PBDR            (*(volatile unsigned char*)0xfffd6)
#define P2PCR           (*(volatile unsigned char*)0xfffd8)
#define P4PCR           (*(volatile unsigned char*)0xfffda)
#define P5PCR           (*(volatile unsigned char*)0xfffdb)

#define ITU_STR         (*(volatile unsigned char*)0xfff60)
#define ITU_SNC         (*(volatile unsigned char*)0xfff61)
#define ITU_MDR         (*(volatile unsigned char*)0xfff62)
#define ITU_FCR         (*(volatile unsigned char*)0xfff63)
#define ITU_TOER        (*(volatile unsigned char*)0xfff90)
#define ITU_TOCR        (*(volatile unsigned char*)0xfff91)

#define ITU0_CNT        (*(volatile unsigned int *)0xfff68)
#define ITU0_TCR        (*(volatile unsigned char*)0xfff64)
#define ITU0_GRA        (*(volatile unsigned int *)0xfff6a)
#define ITU0_GRB        (*(volatile unsigned int *)0xfff6c)
#define ITU0_IER        (*(volatile unsigned char*)0xfff66)
#define ITU0_TSR        (*(volatile unsigned char*)0xfff67)
#define ITU0_TIOR       (*(volatile unsigned char*)0xfff65)

#define ITU1_CNT        (*(volatile unsigned int *)0xfff72)
#define ITU1_TCR        (*(volatile unsigned char*)0xfff6e)
#define ITU1_GRA        (*(volatile unsigned int *)0xfff74)
#define ITU1_GRB        (*(volatile unsigned int *)0xfff76)
#define ITU1_IER        (*(volatile unsigned char*)0xfff70)
#define ITU1_TSR        (*(volatile unsigned char*)0xfff71)
#define ITU1_TIOR       (*(volatile unsigned char*)0xfff6f)

#define ITU2_CNT        (*(volatile unsigned int *)0xfff7c)
#define ITU2_TCR        (*(volatile unsigned char*)0xfff78)
#define ITU2_GRA        (*(volatile unsigned int *)0xfff7e)
#define ITU2_GRB        (*(volatile unsigned int *)0xfff80)
#define ITU2_IER        (*(volatile unsigned char*)0xfff7a)
#define ITU2_TSR        (*(volatile unsigned char*)0xfff7b)
#define ITU2_TIOR       (*(volatile unsigned char*)0xfff79)

#define ITU3_CNT        (*(volatile unsigned int *)0xfff86)
#define ITU3_TCR        (*(volatile unsigned char*)0xfff82)
#define ITU3_GRA        (*(volatile unsigned int *)0xfff88)
#define ITU3_GRB        (*(volatile unsigned int *)0xfff8a)
#define ITU3_BRA        (*(volatile unsigned int *)0xfff8c)
#define ITU3_BRB        (*(volatile unsigned int *)0xfff8e)
#define ITU3_IER        (*(volatile unsigned char*)0xfff84)
#define ITU3_TSR        (*(volatile unsigned char*)0xfff85)
#define ITU3_TIOR       (*(volatile unsigned char*)0xfff83)

#define ITU4_CNT        (*(volatile unsigned int *)0xfff96)
#define ITU4_TCR        (*(volatile unsigned char*)0xfff92)
#define ITU4_GRA        (*(volatile unsigned int *)0xfff98)
#define ITU4_GRB        (*(volatile unsigned int *)0xfff9a)
#define ITU4_BRA        (*(volatile unsigned int *)0xfff9c)
#define ITU4_BRB        (*(volatile unsigned int *)0xfff9e)
#define ITU4_IER        (*(volatile unsigned char*)0xfff94)
#define ITU4_TSR        (*(volatile unsigned char*)0xfff95)
#define ITU4_TIOR       (*(volatile unsigned char*)0xfff93)

#define SCI0_SMR        (*(volatile unsigned char*)0xfffb0)
#define SCI0_BRR        (*(volatile unsigned char*)0xfffb1)
#define SCI0_SCR        (*(volatile unsigned char*)0xfffb2)
#define SCI0_TDR        (*(volatile unsigned char*)0xfffb3)
#define SCI0_SSR        (*(volatile unsigned char*)0xfffb4)
#define SCI0_RDR        (*(volatile unsigned char*)0xfffb5)
#define SCI0_SCMR       (*(volatile unsigned char*)0xfffb6)

#define SCI1_SMR        (*(volatile unsigned char*)0xfffb8)
#define SCI1_BRR        (*(volatile unsigned char*)0xfffb9)
#define SCI1_SCR        (*(volatile unsigned char*)0xfffba)
#define SCI1_TDR        (*(volatile unsigned char*)0xfffbb)
#define SCI1_SSR        (*(volatile unsigned char*)0xfffbc)
#define SCI1_RDR        (*(volatile unsigned char*)0xfffbd)

#define AD_DRA          (*(volatile unsigned int *)0xfffe0)
#define AD_DRB          (*(volatile unsigned int *)0xfffe2)
#define AD_DRC          (*(volatile unsigned int *)0xfffe4)
#define AD_DRD          (*(volatile unsigned int *)0xfffe6)
#define AD_CSR          (*(volatile unsigned char*)0xfffe8)
#define AD_CR           (*(volatile unsigned char*)0xfffe9)

#define DA_STCR         (*(volatile unsigned char*)0xfff5c)
#define DA_DR0          (*(volatile unsigned char*)0xfffdc)
#define DA_DR1          (*(volatile unsigned char*)0xfffdd)
#define DA_CR           (*(volatile unsigned char*)0xfffde)

#define MAR0A           (*(volatile unsigned long*)0xfff20)
#define MAR0AR          (*(volatile unsigned char*)0xfff20)
#define MAR0AE          (*(volatile unsigned char*)0xfff21)
#define MAR0AH          (*(volatile unsigned char*)0xfff22)
#define MAR0AL          (*(volatile unsigned char*)0xfff23)
#define ETCR0A          (*(volatile unsigned int *)0xfff24)
#define IOAR0A          (*(volatile unsigned char*)0xfff26)
#define DTCR0A          (*(volatile unsigned char*)0xfff27)
#define MAR0B           (*(volatile unsigned long*)0xfff28)
#define MAR0BR          (*(volatile unsigned char*)0xfff28)
#define MAR0BE          (*(volatile unsigned char*)0xfff29)
#define MAR0BH          (*(volatile unsigned char*)0xfff2a)
#define MAR0BL          (*(volatile unsigned char*)0xfff2b)
#define ETCR0B          (*(volatile unsigned int *)0xfff2c)
#define IOAR0B          (*(volatile unsigned char*)0xfff2e)
#define DTCR0B          (*(volatile unsigned char*)0xfff2f)

#define MAR1A           (*(volatile unsigned long*)0xfff30)
#define MAR1AR          (*(volatile unsigned char*)0xfff30)
#define MAR1AE          (*(volatile unsigned char*)0xfff31)
#define MAR1AH          (*(volatile unsigned char*)0xfff32)
#define MAR1AL          (*(volatile unsigned char*)0xfff33)
#define ETCR1A          (*(volatile unsigned int *)0xfff34)
#define IOAR1A          (*(volatile unsigned char*)0xfff36)
#define DTCR1A          (*(volatile unsigned char*)0xfff37)
#define MAR1B           (*(volatile unsigned long*)0xfff38)
#define MAR1BR          (*(volatile unsigned char*)0xfff38)
#define MAR1BE          (*(volatile unsigned char*)0xfff39)
#define MAR1BH          (*(volatile unsigned char*)0xfff3a)
#define MAR1BL          (*(volatile unsigned char*)0xfff3b)
#define ETCR1B          (*(volatile unsigned int *)0xfff3c)
#define IOAR1B          (*(volatile unsigned char*)0xfff3e)
#define DTCR1B          (*(volatile unsigned char*)0xfff3f)

#define FLMCR1          (*(volatile unsigned char*)0xfff40)
#define FLMCR2          (*(volatile unsigned char*)0xfff41)
#define EBR             (*(volatile unsigned char*)0xfff42)
#define RAMCR           (*(volatile unsigned char*)0xfff47)

#define DIVCR           (*(volatile unsigned char*)0xfff5d)
#define MSTCR           (*(volatile unsigned char*)0xfff5e)
#define CSCR            (*(volatile unsigned char*)0xfff5f)

#define TPMR            (*(volatile unsigned char*)0xfffa0)
#define TPCR            (*(volatile unsigned char*)0xfffa1)
#define NDERB           (*(volatile unsigned char*)0xfffa2)
#define NDERA           (*(volatile unsigned char*)0xfffa3)
#define NDRB_1          (*(volatile unsigned char*)0xfffa4)
#define NDRA_1          (*(volatile unsigned char*)0xfffa5)
#define NDRB_2          (*(volatile unsigned char*)0xfffa6)
#define NDRA_2          (*(volatile unsigned char*)0xfffa7)

#define TCSR            (*(volatile unsigned char*)0xfffa8)
#define TCNT            (*(volatile unsigned char*)0xfffa9)
#define RSTCSR          (*(volatile unsigned char*)0xfffab)
#define TCSR_W          (*(volatile unsigned int *)0xfffa8)
#define TCNT_W          (*(volatile unsigned int *)0xfffa8)
#define RSTCSR_W        (*(volatile unsigned int *)0xfffaa)

#define RFSHCR          (*(volatile unsigned char*)0xfffac)
#define RTMCSR          (*(volatile unsigned char*)0xfffad)
#define RTCNT           (*(volatile unsigned char*)0xfffae)
#define RTCOR           (*(volatile unsigned char*)0xfffaf)

#define ABWCR           (*(volatile unsigned char*)0xfffec)
#define ASTCR           (*(volatile unsigned char*)0xfffed)
#define WCR             (*(volatile unsigned char*)0xfffee)
#define WCER            (*(volatile unsigned char*)0xfffef)
#define MDCR            (*(volatile unsigned char*)0xffff1)
#define SYSCR           (*(volatile unsigned char*)0xffff2)
#define BRCR            (*(volatile unsigned char*)0xffff3)
#define ISCR            (*(volatile unsigned char*)0xffff4)
#define IER             (*(volatile unsigned char*)0xffff5)
#define ISR             (*(volatile unsigned char*)0xffff6)
#define IPRA            (*(volatile unsigned char*)0xffff8)
#define IPRB            (*(volatile unsigned char*)0xffff9)