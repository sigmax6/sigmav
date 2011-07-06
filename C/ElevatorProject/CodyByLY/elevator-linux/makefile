RELEASE = elevator
DEBUG	= elevator_debug
SOURCE	= elevator.c


release : $(SOURCE)
	gcc $(SOURCE) -o $(RELEASE) -l pthread

debug   : $(SOURCE)
	gcc $(SOURCE) -o $(DEBUG) -l pthread -g

all		: release debug

run		: 
	./$(RELEASE)

gdb		:
	gdb $(DEBUG)

clean	:
	-rm -f $(RELEASE) $(DEBUG)


