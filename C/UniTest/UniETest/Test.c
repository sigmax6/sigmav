#include <stdio.h>

struct Student
{
	int stuId;
	char stuName[10];
};

struct Date
{
	int year;
	int month;
	int day;
};
	
struct StudentEx
{
	int stuId;
	char stuName[10];
	struct Date stuBirth;
};
	 
int main()
{
	printf("size of int is %d\n",sizeof(int));
	struct Student a;
	printf("size of Student is %d\n",sizeof(a));
	struct StudentEx b;
	printf("size of StudentEx is %d\n",sizeof(b));
	return 0;
}