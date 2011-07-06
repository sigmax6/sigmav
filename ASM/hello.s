##############################
## A helloworld ASM program ##
##############################
  .section .data

  .section .text
  .globl _start
_start:
  movl $1,%eax   #linux kernel command number(sys call) for exiting a
                 #program
                 #%eax: holds the system call number
  movl $9,%ebx   #status number return to the os 
                 #change this an it will return different things to 
                 #echo $?
                 #%ebx: holds the return status
  int $0x80      #wakes up the kernel to run the exit command
