void assignstudentstoclasses();
void assignteacherstoclasses();
void modifyteachers();
void endSemester();
void printRolls();
void admissions()
{
    int n;
    char x;
    float cutoff;
    struct student s;
    FILE *f = fopen("applicants.bin","wb");
    printf("Year of joining for new students is %d\n",getyear());
    printf("Change? (y/n): ");
    scanf(" %c",&x);
    if(x=='y')
        nextyear();
    printf("Enter number of applicants: ");
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        printf("Enter the cutoff marks: ");
        scanf("%f",&cutoff);
        if(cutoff>=190)
        {
            // add to file
            printf("Enter your department code: (0 - CSE or 1 - ECE) ");
            // todo: input must be a string, we must assign enum values ourselves.
            scanf("%d",&s.cls.dept);
            printf("Enter name: ");
            scanf(" %[^\n]s",s.name);
            printf("Enter dob (DD-MM-YYYY) : ");
            scanf("%s",s.dob);
            s.cls.cursem=1;
            printf("Enter your email ID: ");
            scanf("%s",s.emailid);
            fwrite(&s,sizeof(s),1,f);
            printf("Application submitted\n\n");
        }
        else
            printf("Rejected\n");
    }
    printf("Admissions over\n");
    fclose(f);
    assignstudentstoclasses();
}

void assignstudentstoclasses()
{
    int ccount=1,ecount=1,year=getyear();
    char csec='A',esec='A';
    char t[10];
    FILE *f=fopen("applicants.bin","rb");
    FILE *csef=fopen("csestudents.bin","ab");
    FILE *ecef=fopen("ecestudents.bin","ab");
    struct student s;
    while(fread(&s,sizeof(s),1,f))
    {
        if(s.cls.dept==cse)
        {
            if(csec=='D')
            {
                //limit of 3 sections per dept has been reached.
                continue;
            }
            s.cls.section=csec;
            s.cls.dept=cse;
            sprintf(s.regno,"%d%s%d",year,getDeptString(s.cls.dept),ccount);
            fwrite(&s,sizeof(s),1,csef);
            ccount++;
            if(ccount>CLASS_STRENGTH)
            {
                csec++;
                ccount=0;
            }
        }
        else if(s.cls.dept==ece)
        {
            if(esec=='D')
                continue;
            s.cls.section=esec;
            s.cls.dept=ece;
            sprintf(s.regno,"%d%s%d",year,getDeptString(s.cls.dept),ccount);
            fwrite(&s,sizeof(s),1,ecef);
            ecount++;
            if(ecount>CLASS_STRENGTH)
            {
                esec++;
                ecount=0;
            }
        }
    }
    printf("Students have been selected into classes.\n");
    fclose(f);
    fclose(csef);
    fclose(ecef);
    assignteacherstoclasses();
}
void admin()
{
    int ch;
    printf("1. Begin admissions process\n");
    printf("2. Print Students roll list\n");
    printf("3. Add/remove teachers\n");
    printf("4. End of semester\n");
    scanf("%d",&ch);
    if(ch==1)
        admissions();
    else if (ch==2)
        printRolls();
    else if(ch==3)
        modifyteachers();
    else if (ch==4)
        endSemester();
}

void assignteacherstoclasses()
{
    //work of this is to summa go through csefaculty and ecefaculty files and assign first 3 to 3 cse and ece classes
    FILE *cteachers=fopen("cseteachers.bin","rb+"),*eteachers=fopen("eceteachers.bin","rb+");
    //FIlE *cstudents=fopen("csestudents.bin","ab+"),*estudents=fopen("ecestudents.bin","ab+");
    struct teacher t;
    char sec='A';
    for(int i=0;i<3;i++)
    {
        fread(&t,sizeof(t),1,cteachers); //read the next cse teacher
        copy(&t.classes[t.nclasses],sec+i,1,cse); //assign the class to this teacher
        t.nclasses++;
        fseek(cteachers,-1*sizeof(t),SEEK_CUR);
        fwrite(&t,sizeof(t),1,cteachers); //update records
        //same crap for next dept
        fread(&t,sizeof(t),1,eteachers);
        copy(&t.classes[t.nclasses],sec+i,1,ece);
        t.nclasses++;
        fseek(eteachers,-1*sizeof(t),SEEK_CUR);
        fwrite(&t,sizeof(t),1,eteachers);
    }
    fclose(eteachers);
    fclose(cteachers);
    printf("Class Teachers have been assigned to the classes\n");
    printf("The portal is now ready to be accessed by students and teachers.\n");
}

void modifyteachers()
{
    // add/remove teachers from workroll
}

void printRolls()
{
    // print out name, roll no for each student in database 
    int ch;
    struct student t;
    FILE *f;
    printf("Enter the department: ");
    scanf("%d",&ch);
    char filename[50];
    strcpy(filename,getDeptString(ch));
    f=fopen(joinStrings(filename,"students.bin"),"rb");
    if(f==NULL)
    {
        printf("No students in record. Begin admissions process first.\n");
        return;
    }
    printf("\tName\t\tReg no.\t   Class\tSem\n");
    while(fread(&t,sizeof(t),1,f))
    {
        printf("%20s %10s %7s-%c\t%d\n",t.name,t.regno,getDeptString(ch),t.cls.section,t.cls.cursem);
    }
}

void endSemester()
{printf("ok\n");
    //move each student to the next semester. if 8 semesters complete, delete from the record.
    FILE *c=fopen("csestudents.bin","rb"),*ct=fopen("tcse.bin","wb");
    FILE *e=fopen("ecestudents.bin","rb"),*et=fopen("tece.bin","wb");
    struct student obj;
    struct teacher tobj;
    int i,size;
    while(fread(&obj,sizeof(obj),1,c))
    {
        obj.cls.cursem++;
        if(obj.cls.cursem!=9)
            fwrite(&obj,sizeof(obj),1,ct);
    }
    //see if a new academic year has started. If yes, change system date
    if(obj.cls.cursem%2==1)
        nextyear(); 
    fclose(c);
    fclose(ct);
    remove("csestudents.bin");
    rename("tcse.bin","csestudents.bin");
    while(fread(&obj,sizeof(obj),1,e))
    {
        obj.cls.cursem++;
        if(obj.cls.cursem!=9)
            fwrite(&obj,sizeof(obj),1,et);
    }
    fclose(e);
    fclose(et);
    remove("ecestudents.bin");
    rename("tece.bin","ecestudents.bin");

    //same procedure for teachers
    c=fopen("cseteachers.bin","rb+"); 
    while(fread(&tobj,sizeof(tobj),1,c))
    {
        // increment cursem for all classes this teacher is incharge of
        // if class is already in 8th semester, delete it from classes array
        // but that class may be in the middle of the array and we cant just leave a space in the middle...
        // so create another classes array and copy each class into it, excluding the 8th semester ppl
        // then replace the array
        printf("Inside while loop: empid = %s\n",tobj.empid);
        scanf(" %*c");
        struct class newclasses[5];
        int size=0;
        for(i=0;i<tobj.nclasses;i++)
            if(tobj.classes[i].cursem!=8)
            {
                tobj.classes[i].cursem++;
                newclasses[size++]=tobj.classes[i];
            }
        // replace classes[] and nclasses in the teacher object
        tobj.nclasses=size;
        for(i=0;i<size;i++)
            tobj.classes[i]=newclasses[i];
        //update the teacher file
        fseek(c,-1*sizeof(tobj),SEEK_CUR);
        fwrite(&tobj,sizeof(tobj),1,c);
    }
    fclose(c);
    fclose(ct);

    //repeat for next department
    c=fopen("eceteachers.bin","rb+");
    while(fread(&tobj,sizeof(tobj),1,c))
    {
        struct class newclasses[5];
        int size=0;
        for(i=0;i<tobj.nclasses;i++)
            if(tobj.classes[i].cursem!=8)
            {
                tobj.classes[i].cursem++;
                newclasses[size++]=tobj.classes[i];
            }
        // replace classes[] and nclasses in the teacher object
        tobj.nclasses=size;
        for(i=0;i<size;i++)
            tobj.classes[i]=newclasses[i];
        //update the teacher file
        fseek(c,-1*sizeof(tobj),SEEK_CUR);
        fwrite(&tobj,sizeof(tobj),1,c);
    }
    fclose(c);
    fclose(ct);
}