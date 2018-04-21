#define CLASS_STRENGTH 50
char* joinStrings(char a[], char b[])
{
	strcat(a,b);
	return a;
}

int comp(struct class c1, struct  class c2)
{
	return(c1.dept==c2.dept&&c1.cursem==c2.cursem&&c1.section==c2.section);	
}

void copy(struct class *obj,char sec, int cursem, int dept)
{
	obj->section=sec;
	obj->cursem=cursem;
	obj->dept=dept;
}

char* getDeptString(int dept)
{
	if(dept==cse)
		return "cse";
	else if(dept==ece)
		return "ece";
}

int getyear()
{
	int year;
	FILE *f=fopen("firstrun","r");
	fscanf(f,"%d",&year);
	printf("Value of year is %d\n",year);
	fclose(f);
	return year;
}
void nextyear()
{
	int year=getyear()+1;
	FILE *f=fopen("firstrun","w");
	fprintf(f, "%d", year);
	fclose(f);
}