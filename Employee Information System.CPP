#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define SHORT_SIZE 2               /* Number of bytes in short data type */
#define MAX_CHRS 50       /* Maximum length for a title, less one for \0 */
#define ITEMS 3              /* Number of data items kept for each video */

			  /* Indices for data items array for each video */
   #define AGE 0
   #define NUMBER 1                                        /* In minutes */
   #define POSITION 2                            /* Position on the shelves */
   #define SALARY 3                            /* Position on the shelves */

void add(void);
void del(void);
void countrecord(void);
void change_title(void);
void change(int sub);
void print_file(void);
void write_record(FILE *video, char title[], short age,
		  short number, short position, short salary);
void new_file(void);

void main(void)
{  FILE *video;
	char choice;
	char firstrun;


/* For First Run purpose */
	puts("****** Welcome to Employee Information System ******");
	printf("Press <ENTER> to continue...");
	firstrun = toupper(getchar());
	fflush(stdin);
	clrscr();
	if(firstrun == 'Y')
	{
		new_file();             /* Used only for the first run of the test */
	}

	/***************************************** Test to see if file exists */
	if ((video = fopen("VIDEOS.DAT", "rb")) == NULL)
	{  puts("Video file missing.");
		exit(EXIT_FAILURE);
	}
	fclose(video);

	do /************************************************************ Menu */
	{
		puts("Do you want to:				    Change:");
		puts("  [A]dd new employee?			    A [N]ame of the Employee:");
		puts("  [R]emove this account of the employee?      A [Y]ears old of the employee:");
		puts("  [P]rint the list of employee?		    A [I]D Number:");
		puts("  [C]ount Record of employee?		    A [D]esignation position of the employee:");
		puts("  [E]xit?					    A [S]alary: ");
		printf("Take your choice: ");
		choice = toupper(getchar());
		fflush(stdin);                     /* Or while (getchar() != '\n') */

		switch (choice)
		{  case 'A':
				add();
				break;
			case 'R':
				del();
				break;
			case 'N':
				change_title();
				break;
			case 'Y':
				change(AGE);
				break;
			case 'I':
				change(NUMBER);
				break;
			case 'D':
				change(POSITION);
				break;
			case 'S':
				change(SALARY);
				break;
			case 'C':
				countrecord();
				break;
			case 'P':
				print_file();
		}
		puts("Press <ENTER> to continue ...");
		getch();
		clrscr();
	}while (choice != 'E');

	puts("Program Ended ...");
	getch();
}

void new_file(void) /************* Creates new file for testing purposes */
{  FILE *video;

	video = fopen("VIDEOS.DAT", "wb");
	fclose(video);
}

void write_record /********************** Writes a single record to file */
	(FILE *video, char title[], short age, short number, short position, short salary)
{  fputs(title, video);
	fputs("\n", video);    /* Newline at end of title so that fgets works */
	fwrite(&age, SHORT_SIZE, 1, video);
	fwrite(&number, SHORT_SIZE, 1, video);
	fwrite(&position, SHORT_SIZE, 3, video);
	fwrite(&salary, SHORT_SIZE, 3, video);
}

void print_file(void) /******************* Displays the data in the file */
{  FILE *video;
	char title[MAX_CHRS];
	short data[ITEMS], sub;

	video = fopen("VIDEOS.DAT", "rb");
	puts("\n  Age ID  Employee_Position Salary Employee Name");
	while (fgets(title, MAX_CHRS, video) != NULL)
	{  fread(data, SHORT_SIZE, ITEMS, video);
		for (sub = 0; sub < ITEMS; ++sub)
			printf("%6i", data[sub]);
		printf("   %s", title);
	}
	puts("");
	fclose (video);
}

void countrecord(void) /* Count Record */
{  FILE *video;
	char title[MAX_CHRS];
	short data[ITEMS], sub;
	int i=0;		/* record counter */

	video = fopen("VIDEOS.DAT", "rb");
	while (fgets(title, MAX_CHRS, video) != NULL)
	{  fread(data, SHORT_SIZE, ITEMS, video);
		i++;
	}
	printf("Number of employee : %d \n",i);
	fclose (video);
}

void add(void) /********************************* Adds a record in order */
{  FILE *video, *temp;
   char title[MAX_CHRS], new_title[MAX_CHRS];
	short data[ITEMS];
   short age, number, position,salary;

   video = fopen("VIDEOS.DAT", "rb");
	temp = fopen("TEMP.$TM", "wb");

   printf("Name of Employee: ");
   gets(new_title);
   printf("Age, ID number ,Employee Position, Salary: ");
   scanf("%hi %hi %hi", &age, &number, &position, &salary);
   while (getchar() != '\n');                   /* Dump trailing newline */

   while (fgets(title, MAX_CHRS, video) != NULL)
   {  fread(data, SHORT_SIZE, ITEMS, video);
      if (strcmp(new_title, title) < 0)          /* Should use all ucase */
      {  write_record(temp, new_title, age, number, position,salary);
	 new_title[0] = '~';         /* Highest chr, won't find it again */
		}
      fputs(title, temp);
      fwrite(data, SHORT_SIZE, ITEMS, temp);
   }

   if (new_title[0] != '~')                        /* Add to end of file */
      write_record(temp, new_title, age, number, position,salary);

   fclose(video);
   fclose(temp);
   remove("VIDEOS.DAT");
	rename("TEMP.$TM", "VIDEOS.DAT");
}

void del(void) /************************* Deletes a record from the file */
{  FILE *video, *temp;
   char title[MAX_CHRS], del_title[MAX_CHRS], answer;
   short data[ITEMS];

   video = fopen("VIDEOS.DAT", "rb");
   temp = fopen("TEMP.$TM", "wb");

   printf("Delete old employee: ");
   gets(del_title);
   while (fgets(title, MAX_CHRS, video) != NULL)
   {  fread(data, SHORT_SIZE, ITEMS, video);
      answer = 'N';                           /* Default to don't delete */
      if (strncmp(del_title, title, strlen(del_title)) == 0)
											 /* Test as many characters as typed in */
      {  printf("Delete this account %.*s? [Y/N]",           /* Print without newline */
                strlen(title) - 1, title);
         answer = toupper(getchar());
         while (getchar() != '\n');          /* Dump newline from stream */
      }

		if (answer != 'Y')                         /* If not delete, write */
		{  fputs(title, temp);
         fwrite(data, SHORT_SIZE, ITEMS, temp);
      }
		else                                         /* Have made deletion */
         del_title[0] = '~';                      /* Don't find it again */
   }

   fclose(video);
   fclose(temp);
   remove("VIDEOS.DAT");
   rename("TEMP.$TM", "VIDEOS.DAT");
}

void change_title(void) /******************************* Changes a title */
{  FILE *video, *temp;
   char title[MAX_CHRS], ch_title[MAX_CHRS], answer;
   short data[ITEMS];

   video = fopen("VIDEOS.DAT", "rb");
   temp = fopen("TEMP.$TM", "wb");

   printf("Change employee: ");
   gets(ch_title);
   while (fgets(title, MAX_CHRS, video) != NULL)          /* Read record */
	{  fread(data, SHORT_SIZE, ITEMS, video);
      if (strncmp(ch_title, title, strlen(ch_title)) == 0)
                                  /* Test as many characters as typed in */
		{  printf("Change %.*s? [Y/N] ",          /* Print without newline */
					 strlen(title) - 1, title);
			answer = getchar();
			while (getchar() != '\n');    /* Dump newline from input stream */
			if (toupper(answer) == 'Y')
			{  printf("To: ");
				gets(title);               /* Change the title in the record */
				strcat(title, "\n");                 /* Add newline for file */
				ch_title[0] = '~';                    /* Don't find it again */
			}
		}
		fputs(title, temp);                                /* Write record */
		fwrite(data, SHORT_SIZE, ITEMS, temp);
	}

	fclose(video);
	fclose(temp);
	remove("VIDEOS.DAT");
	rename("TEMP.$TM", "VIDEOS.DAT");
}

void change(int sub) /************** Changes one of the other parameters */
{  FILE *video;
	char title[MAX_CHRS], ch_title[MAX_CHRS], answer;
	short data[ITEMS];

	video = fopen("VIDEOS.DAT", "rb+");            /* Must read and write */

	printf("Change: ");
	gets(ch_title);
	while (fgets(title, MAX_CHRS, video) != NULL)          /* Read record */
	{  fread(data, SHORT_SIZE, ITEMS, video);
		if (strncmp(ch_title, title, strlen(ch_title)) == 0)
											 /* Test as many characters as typed in */
		{  printf("Change %.*s? [Y/N]",           /* Print without newline */
					 strlen(title) - 1, title);
			answer = getchar();
			while (getchar() != '\n');          /* Dump end of input stream */
			if (toupper(answer) == 'Y')          /* Confirm if proper title */
         {  printf("Change %i to: ", data[sub]);
            scanf("%i", &data[sub]);
            fflush(stdin);                   /* Dump end of input stream */
            fseek(video, -(long)(ITEMS * SHORT_SIZE), SEEK_CUR);/*Back up*/
            fwrite(data, SHORT_SIZE, ITEMS, video);
            fseek(video, 0L, SEEK_END);               /* Force loop exit */
			}
      }
   }
   fclose(video);
}
