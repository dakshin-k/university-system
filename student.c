void student()
{
	FILE *f;
	int dept; 
	char regno[5],filename[20];
	printf("Enter your department: ");
	scanf(" %d",&dept);
	strcpy(filename,getDeptString(dept));
	f=fopen(joinStrings(filename,"students.bin"),"rb+");
	if(f==NULL)
	{
		printf("No students are currently enrolled. Please conduct admissions first.\n");
		return;
	}
	
}