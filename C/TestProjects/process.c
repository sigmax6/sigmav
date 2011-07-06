/**
 * Copyright LuXiaoShuang
 *
 * All rights reserved.
**/

#include <stdio.h>
#include <time.h>

// clock() number of clock ticks since the program start

int delay(size_t second)
{
///*
	time_t head, tail;

	time(&head);

	do
	{
		time(&tail);
	}
	while(difftime(tail, head) < second);
//*/
/*
	clock_t deadline;

	deadline = clock() + second * CLOCKS_PER_SEC;

	while(clock() < deadline)
	{
		;
	}
*/
	return 0;
}

int main()
{
	int i, j, m, n;

	n = 10;

//	printf("%ld\n", CLOCKS_PER_SEC);

	for(i = 0; i <= n; ++ i)
	{
		m = 10 * i / n;

		printf("\r");

		for(j = 0; j < m; ++ j)
		{
			printf("-");
		}

		printf("\b>");

		for(j = m; j < n; ++ j)
		{
			printf(" ");
		}

		printf(" %c %d", "-\\|/"[i % 4], m);

        fflush(stdout);

		delay(1);
	}

	printf("\n\v\f\n");

	return 0;
}
