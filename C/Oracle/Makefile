#Make file Template for Pro C by Sigmax6 2010

include $(ORACLE_HOME)/rdbms/lib/env_rdbms.mk

ORAINC=${ORACLE_HOME}/precomp/public
ORALIB=${ORACLE_HOME}/lib

PROC=proc
CC=cc
PROCPPFLAGS= \
	userid=scott/hotel000 \
	sqlcheck=semantics \
	include=${ORACLE_HOME}/rdbms/public \
    include=$(ORAINC) \
	lines=ture  \
	parse=full \ 
    mode=oracle

CFLAGS= -I$(ORAINC) -I${ORACLE_HOME}/rdbms/public

.SUFFIXES: .o .c .pc
.pc.c:
	@$(PROC) $(PROCPPFLAGS) iname=$<
.c.o:
	@$(CC) $(CFLAGS) -c $<
.pc.o:
	@$(PROC) $(PROCPPFLAGS) iname=$<
	@$(CC) $(CFLAGS) -c $(<:.pc=.c)


###############   USER DEFINE HERE ###############
OBJ=main.o
main:$(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o main -L${ORACLE_HOME}/lib $(PROLDLIBS) -lm
	@rm -f $(OBJ)
all:
	@make main 

