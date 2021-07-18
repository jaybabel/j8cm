// j8cm_ldr.cpp


void read_obj(char *fnm)
{
  FILE *infp;
  char fnm_x[PATHLEN];
  int ac, i, d;
  struct word_node to_mem;

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".obj");
  clrscr();
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
  clrscr();
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
  clrscr();
  printf("Enter device name>");
  scanf("%s", &p);
  strcpy(pn, p);
}

void loader()
{
  char pathname[PATHLEN], f[PATHLEN], fname[9];

  choice = 0;
  strcpy(pathname, "a:");
  while (choice != 52) {
	 clrscr();
	 printf("       J8CM Loader Utility Menu   \n\n");
	 printf("   1. Load '.obj' file into memory\n");
	 printf("   2. Load '.ldr' file into I/O in\n");
	 printf("   3. Set device name(%s)\n", pathname);
	 printf("   4. Return to main menu\n");
	 choice = getch();
	 if (choice == 49) {
		printf("\nEnter name(without extender) of file to load - %s",
				 pathname);
		scanf("%s", &fname);
		strcpy(f, pathname);
		strcat(f, fname);
		read_obj(f);
	 }
	 if (choice == 50) {
		printf("\nEnter name(without extender) of file to load - %s",
				 pathname);
		scanf("%s", &fname);
		strcpy(f, pathname);
		strcat(f, fname);
		read_ldr(f);
	 }
	 if (choice == 51) setdir(pathname);
  }
  choice = 0;
}
