//let's first create a file of teachers who are already existing.
void firstrun()
{
	FILE *check=fopen("firstrun","r");
	if(check!=NULL)
		return;
	printf("Program is running for the first time\n");
	FILE *f=fopen("cseteachers.bin","wb");
	struct teacher t;
	printf("Creating CSE Teachers database\n");
	char ch='y';
	do
	{
	printf("Enter empid: ");
	scanf(" %s",t.empid);
	printf("Enter name: ");
	scanf(" %s",t.name);
	t.dept=cse;
	t.nclasses=0;
	fwrite(&t,sizeof(t),1,f);
	printf("Continue? y/n");
	scanf(" %c",&ch);
	} while(ch!='n');
	fclose(f);
	ch='y';
	f=fopen("eceteachers.bin","wb");
	printf("Creating ECE Teachers database\n");
	do
	{
	printf("Enter empid: ");
	scanf(" %s",t.empid);
	printf("Enter name: ");
	scanf(" %s",t.name);
	t.dept=ece;
	t.nclasses=0;
	fwrite(&t,sizeof(t),1,f);
	printf("Continue? y/n");
	scanf(" %c",&ch);
	} while(ch!='n');
	check=fopen("firstrun","w");
	//get current year and store it in firstrun
	time_t timeobj=time(NULL);
	struct tm tm=*localtime(&timeobj);
	printf("Current date is %d\n",tm.tm_year+1900 );
	fprintf(check, "%d", tm.tm_year+1900);
	fclose(check);
	fclose(f);
}