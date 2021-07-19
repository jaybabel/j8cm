// j8cm_ldr.cpp
#include <dirent.h>

void listfiles(char *p)
{
  char c;
  DIR *d;
   struct dirent *dir;
//   d = opendir(".");
   d = opendir(p);
   if (d)
   {
       while ((dir = readdir(d)) != NULL)
       {
           printf("%s\n", dir->d_name);
       }
       closedir(d);
    }
  set_conio_terminal_mode();
    c = getchar();
  reset_terminal_mode();
}

void read_obj(char *fnm)
{
  FILE *infp;
  char fnm_x[PATHLEN];
  int ac, i, d;
  struct word_node to_mem;

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".obj");
  system("clear");
  printf("%s\n", fnm_x);
  if((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open .obj file\n");
  }
  else {
	 fscanf(infp, "%d%d", &ac, &d);
	 while (!feof(infp)) {
		to_mem = twos_comp(d);
		for (i=1; i<9; ++i)
		  memory[ac] [i] = to_mem.temp_word[i];
		fscanf(infp, "%d%d", &ac, &d);
	 }
	 fclose(infp);
  }
}                                // end read_obj

void read_ldr(char *fnm)
{
  FILE *infp;
  char fnm_x[PATHLEN];
  int bc, i, d;
  struct word_node to_io;

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".ldr");
  system("clear");
  printf("%s\n", fnm_x);
  if((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open .ldr file\n");
  }
  else {
	 bc=0;
	 fscanf(infp, "%d", &d);
	 while (!feof(infp)) {
		to_io = twos_comp(d);
		for (i=1; i<9; ++i)
		  io_in[bc] [i] = to_io.temp_word[i];
		bc++;
		fscanf(infp, "%d", &d);
	 } // end while !EOF
	 fclose(infp);
  }
}                                // end read_ldr

void setdir(char *pn)
{
  char p[32];
  system("clear");
  printf("Enter path name>");
  scanf("%s", p);
  strcpy(pn, p);
}

void loader()
{
  char pathname[PATHLEN], f[PATHLEN], fname[9];

  choice = 0;
  strcpy(pathname, "/home/jay/repos/j8cm_local/j8_tests/");
  while (choice != 54) {
  system("clear");
	 printf("       J8CM Loader Utility Menu   \n\n");
	 printf("   1. Load '.obj' file into memory\n");
	 printf("   2. Load '.ldr' file into I/O in\n");
	 printf("   3. Set file path(%s)\n", pathname);
   printf("   4. List files\n");
   printf("   6. Return to main menu\n");
   set_conio_terminal_mode();
	 choice = getchar();
   reset_terminal_mode();
	 if (choice == 49) {
		printf("\nEnter name(without extender) of file to load - %s",
				 pathname);
		scanf("%s", fname);
		strcpy(f, pathname);
		strcat(f, fname);
		read_obj(f);
	 }
	 if (choice == 50) {
		printf("\nEnter name(without extender) of file to load - %s",
				 pathname);
		scanf("%s", fname);
		strcpy(f, pathname);
		strcat(f, fname);
		read_ldr(f);
	 }
	 if (choice == 51) setdir(pathname);
   if (choice == 52) {
     printf("\nHere is a list of files in %s", pathname);
     listfiles(pathname);
   }
  }
  system("clear");
  choice=0;
}
