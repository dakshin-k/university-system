#include <stdio.h>
#include <string.h>
#include <time.h>
#include "structuredefs.c"
#include "macros.c"
#include "firstrun.c"
#include "admin.c"
#include "teacher.c"
#include "student.c"
int main()
{
	int ch;
	firstrun(); // if teachers database does not already exist, it is created.
	printf("Main menu\n");
	printf("1. Admin mode\n");
	printf("2. Teacher login\n");
	printf("3. Student login\n");
	scanf("%d",&ch);
	switch(ch)
	{
		case 1:
		admin();
		break;
		case 2:
		teacher();
		break;
		case 3:
		student();
	}
	return 0;
}