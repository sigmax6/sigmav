#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define STOP	0
#define UP		1
#define DOWN	2 

#define NONE	0
#define INER	1
#define OUTER	2

#define NUM		3

// array[0] not used
static volatile int up[NUM + 1] = {0};
static volatile int down[NUM + 1] = {0};

static volatile int floor = 3;
static volatile int state = STOP;	

static volatile int open  = 0;
static volatile int close = 0;

//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int get_max_floor()
{
	int i = 0;
	for(i = NUM; i >= 1; i--)
	{
		if(up[i] != 0)
		{ return i; }
	}
	return 0;
}

int get_min_floor()
{
	int i = 0;
	for(i = 1; i <= NUM; i++)
	{
		if(down[i] != 0)
		{ return i; }
	}
	return NUM + 1;
}

void get_input(void)
{
	int f = 0;
	int pos = NONE;	
	int new_floor = 0;
	int new_dir = STOP;
	
	while(1)
	{
		scanf("%d", &f); 
 		pos = NONE; 	
 			
		switch(f)
		{
		case 1 : 
		case 2 :
		case 3 :
			pos = INER;
			new_floor = f;
			printf("!iner user go to #%d\n", new_floor);
			break;
		case 11:	
			pos = OUTER;
			new_floor = 1;
			new_dir = UP;
			printf("!outer user at #%d go UP\n", new_floor);
			break;
		case 19:
			pos = OUTER;
			new_floor = 2;
			new_dir = DOWN;
			printf("!outer user at #%d go DOWN\n", new_floor);
			break;
		case 21:
			pos = OUTER;
			new_floor = 2;
			new_dir = UP;
			printf("!outer user at #%d go UP\n", new_floor);
			break;
		case 29:
			pos = OUTER;
			new_floor = 3;
			new_dir = DOWN;
			printf("!outer user at #%d go DOWN\n", new_floor);
			break;
		case 9:
			open = 1;
			break;
		case 0:
			close = 1;
			break;
		default:break;
		}
	
//		pthread_mutex_lock(&mutex);
		if(pos == OUTER)
		{
			if(state == UP)
			{
				if(new_dir == UP)
				{
					if(new_floor >= floor)
					{ up[new_floor] = 1; }
					else
					{ down[new_floor] = 1; }
				}
				else if(new_dir == DOWN)
				{
					if(new_floor >= get_max_floor())
					{ up[new_floor] = 1; }
					else
					{ down[new_floor] = 1; } 
				}
			}
			else if(state == DOWN)
			{
				if(new_dir == UP)
				{
					if(new_floor <= get_min_floor())
					{ down[new_floor] = 1; }
					else
					{ up[new_floor] = 1; }
				}
				else if(new_dir == DOWN)
				{
					if(new_floor <= floor)
					{ down[new_floor] = 1; }
					else
					{ up[new_floor] = 1; }
				}
			}
			else if(state == STOP)
			{
				if(new_floor >= floor)
				{ up[new_floor] = 1; }
				else
				{ down[new_floor] = 1; }
			}
		}
		else if(pos == INER)
		{
			if(state == UP || state == STOP)
			{
				if(new_floor >= floor)
				{ up[new_floor] = 1; }
				else
				{ down[new_floor] = 1; }
			}
			else if(state == DOWN)
			{
				if(new_floor <= floor)
				{ down[new_floor] = 1; }
				else
				{ up[new_floor] = 1; }
			}
		}
	//	pthread_mutex_unlock(&mutex);
	}
}

void move_up(void)
{
	int i = 0;
	state = UP; 

	printf("#%d : up\n", floor);
	for(i = 0; i < 5; i++)
	{
		sleep(1);
		printf("*\n");		
	}
//	pthread_mutex_lock(&mutex);
		
	if(++floor > NUM)
	{ 
		floor = NUM; 
		state = STOP;	
	}
	//pthread_mutex_unlock(&mutex);
}

void move_down(void)
{
	int i = 0;
	state = DOWN; 	

	printf("#%d : down\n", floor);
	for(i = 0; i < 5; i++)
	{
		sleep(1);
		printf("*\n");
	}
//	pthread_mutex_lock(&mutex);
	
	if(--floor < 1)
	{ 
		floor = 1;
		state = STOP;
	}
//	pthread_mutex_unlock(&mutex);
}

void move_stop(void)
{
	//pthread_mutex_lock(&mutex);
	state = STOP;
	//pthread_mutex_unlock(&mutex);
	printf("#%d : stop\n", floor);	
}

void open_door(int direction, int index)
{
	int i = 0;
	//pthread_mutex_lock(&mutex);
	if(direction == UP)
	{ up[index] = 0; }
	else if(direction == DOWN)
	{ down[index] = 0; }
//	pthread_mutex_unlock(&mutex);
	if(direction != NONE)
	{ printf("#%d : arrive\n", floor); }
	printf("#%d : door open\n", floor);
	for(i = 0; i < 5; i++)
	{	
		if(open)
		{ 
			i = 0;
			open = 0;			
		}
		else if(close)
		{		
			close = 0;
			break;			
		}
		
		sleep(1);
		printf(".");
	}
	printf("\n");
	printf("#%d : door close\n", floor);
}

int get_up_next_floor(void)
{
	int i = 0;
	//pthread_mutex_lock(&mutex);
	for(i = floor; i <= NUM; i++)
	{
		if(up[i] != 0)
		{ return i; }
	}
	//pthread_mutex_unlock(&mutex);
	return 0;
}

int get_down_next_floor(void)
{	
	int i = 0;
	//pthread_mutex_lock(&mutex);
	for(i = floor; i >= 1; i--)
	{
		if(down[i] != 0)
		{ return i; }
	}
	//pthread_mutex_unlock(&mutex);
	return NUM + 1;
}

void show_message(void)
{	
	int loop = 5;
	int new_get = 0;
	int des_get = 0;
	int next_floor = 0;
	
	printf("#%d : stop\n", floor);

	while(1)
	{
		if(state == UP)
		{
			next_floor = get_up_next_floor();
			if(next_floor == floor)
			{ open_door(UP, next_floor); }
			else if(next_floor > floor)
			{ move_up(); }
			else if(next_floor < 1)
			{ move_stop(); }
		}
		else if(state == DOWN)
		{
			next_floor = get_down_next_floor();
			if(next_floor == floor)
			{ open_door(DOWN, next_floor); }
			else if(next_floor < floor)
			{ move_down(); }
			else if(next_floor > NUM)
			{ move_stop(); }
		}
		else if(state == STOP)
		{
			if(open)
			{
				open_door(NONE, 0);
				open = 0;
			}
			else
			{
				next_floor = get_up_next_floor();
				if(next_floor == floor)
				{ open_door(UP, next_floor); }
				else if(next_floor > floor)
				{ move_up(); }
				else if(next_floor < 1)
				{
					next_floor = get_down_next_floor();
					if(next_floor == floor)
					{ open_door(DOWN, next_floor); }
					else if(next_floor < floor)
					{ move_down(); }				
				}
			}
		}
	}	
}

int main(void)
{
	pthread_t id;
	setbuf(stdout, NULL);
	
	if( pthread_create (&id, NULL, (void *)get_input, NULL) )
	{
		perror("Create thread fail!\n");
		exit(EXIT_FAILURE);
	}

	show_message();

	pthread_join(id, NULL);
	return EXIT_SUCCESS;
}
