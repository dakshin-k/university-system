struct class
{
	char section;
	int cursem;
	int dept;
};
struct student
{
	char name[50];
	char regno[18];
	struct class cls;
	char dob[10];
	char emailid[20];
	int marks [3][6]; // 3 UTs ; 6 subjects
	int attendance[6][2]; //6 rows for subjects. 2 cols - attended periods, total periods
};

struct teacher
{	
	char empid[6];
	char name[50];
	int dept;
	int nclasses;
	struct class classes[5];
};

enum depts
{
	cse,ece
};