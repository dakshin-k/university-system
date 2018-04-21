void updatemarks(FILE*,struct class,int);
void updateattendance(FILE *,struct class,int);
void moderate(FILE*,struct class,int);
void teacher()
{
	printf("LOGIN to Teachers Portal\n");
	printf("Enter empid: ");
	char empid[6];
	int dept,clsno,utno,i;
	char filename[100];
	scanf(" %s",empid);
	printf("Enter the department: 0 - cse and 1 - ece\n");
	scanf("%d",&dept);
	strcpy(filename,getDeptString(dept));
	strcat(filename,"teachers.bin");
	printf("filename %s\n",filename);
	FILE *tfile=fopen(filename,"rb");
	int found=0;
	struct teacher tobj;
	while(fread(&tobj,sizeof(tobj),1,tfile))
	{printf("%s\n",tobj.empid);	if(!strcmp(empid,tobj.empid))
		{
			found=1;
			break;
		}}
	if(!found)
	{
		printf("Empid not found in records. \n");
		return;
	}
	fclose(tfile);
	printf("Welcome %s\n",tobj.name);
	printf("You are in charge of %d classes\n",tobj.nclasses);
	//todo error reporting: must exit if nclasses==0
	for(i=0;i<tobj.nclasses;i++)
		printf("%d. %s %c , semester %d\n",i+1,getDeptString(tobj.dept),tobj.classes[i].section,tobj.classes[i].cursem);
	printf("Enter the class number: ");
	scanf("%d",&clsno);
	//open the students file that this character is in charge of
	strcpy(filename,getDeptString(tobj.dept));
	strcat(filename,"students.bin");
	tfile=fopen(filename,"rb+");	
	printf("1. Update UT marks\n");
	printf("2. Update Attendance\n");
	printf("3. Moderate students marks/attendance\n");
	int ch;
	scanf("%d",&ch);
	if(ch==1)
	{
		// updatemarks();
		printf("Enter reporting period no. (UT 1 - 3): ");
		scanf("%d",&utno);
		updatemarks(tfile,tobj.classes[clsno-1],utno);
	}
	else if (ch==2)
	{
		// updateattendance();
		printf("Enter reporting period no. (UT 1 - 3): ");
		scanf("%d",&utno);
		updateattendance(tfile,tobj.classes[clsno-1],utno);
	}
	else if(ch==3)
	{
		// moderate();
		printf("Enter reporting period no. (UT 1 - 3): ");
		scanf("%d",&utno);
		moderate(tfile,tobj.classes[clsno-1],utno);
	}
	else
		printf("Invalid choice.\n");
	fclose(tfile);
}

void updatemarks(FILE *f, struct class cls, int utno)
{
	int i,semno;
	//move pointer to begging of this class's records
	struct student sobj;
	fread(&sobj,sizeof(sobj),1,f); //read in the first record
	semno=sobj.cls.cursem;
	fseek(f,(semno-cls.cursem)/2*(cls.section-'A')*sizeof(struct student)*CLASS_STRENGTH,SEEK_SET);
	while(fread(&sobj,sizeof(sobj),1,f)&&sobj.cls.section==cls.section)
	{
		printf("Student Details\n");
		printf("%s | %s",sobj.name,sobj.regno);
		printf("UT No. %d\n Enter marks in 6 subjects: \n",utno);
		for(i=0;i<6;i++)
			scanf(" %d",&sobj.marks[utno-1][i]);
		fseek(f,-1*sizeof(sobj),SEEK_CUR);
		fwrite(&sobj,sizeof(sobj),1,f);
	}
}
void updateattendance(FILE *f, struct class cls, int utno)
{
	int i,p,t,semno;
	struct student sobj;
	//move pointer to begging of this class's records
	fread(&sobj,sizeof(sobj),1,f); //read in the first record
	semno=sobj.cls.cursem;
	fseek(f,(semno-cls.cursem)/2*(cls.section-'A')*sizeof(struct student)*CLASS_STRENGTH,SEEK_SET);
	while(fread(&sobj,sizeof(sobj),1,f)&&sobj.cls.section==cls.section)
	{
		printf("Student Details\n");
		printf("%s | %s",sobj.name,sobj.regno);
		printf("UT No. %d\n Enter number of attended and total periods: \n",utno);
		for(i=0;i<6;i++)
		{
			scanf(" %d %d",&p,&t);
			sobj.attendance[utno-1][0]+=p;
			sobj.attendance[utno-1][0]+=t;
		}
		fseek(f,-1*sizeof(sobj),SEEK_CUR);
		fwrite(&sobj,sizeof(sobj),1,f);
	}
}
void moderate(FILE *f, struct class cls, int utno)
{
	int c,i;
	struct student t;
	fseek(f,(cls.section-'A')*sizeof(struct student)*CLASS_STRENGTH,SEEK_SET);
	printf("1. Moderate marks\n");
	printf("2. Moderate attendance\n");
	scanf("%d",&c);
	if(c==1)
	{
		while(fread(&t,sizeof(t),1,f)&&comp(cls,t.cls))
		{
			for(i=0;i<6;i++)
			{
				if(t.marks[utno-1][i]<50)
					t.marks[utno-1][i]=50;
				else if(t.marks[utno-1][i]>=75)
					t.marks[utno-1][i]=100;
				else
					t.marks[utno-1][i]*=4/3;
			}
			//update student record with modified marks
			fseek(f,-1*sizeof(t),SEEK_CUR);
			fwrite(&t,sizeof(t),1,f);
		}
	}	
	else if(c==2)
	{
		while(fread(&t,sizeof(t),1,f))
		{
			for(i=0;i<6;i++)
			{
				if((float)t.attendance[i][0]/t.attendance[i][1]<0.75)
					t.attendance[i][0]=0.8*t.attendance[i][1]; //80% to all
			}
			//update student record with modified attendance
			fseek(f,-1*sizeof(t),SEEK_CUR);
			fwrite(&t,sizeof(t),1,f);
		}
	}
	else printf("Invalid choice\n");
}