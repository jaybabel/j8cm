// j8cm_mem.cpp
// memory mode source code
// Jay's Electronic Brain Model
// Jay Babel
// cmsc265
// Spring 1995

                                               // memory mode routines
void disp_mem_pg()
{
  int i, j, k, l, m;
  i=0;
  for (m=1; m<5; ++m) {
//    clrscr();
    printf("\033[0K");
    printf("Memory Contents, Page %d\n", m-1);
    printf("--------------------------\n");
    for (l=1; l<17; ++l) {
      printf("%03d:  ", i);
      for (k=1; k<5; ++k) {
        for (j=1; j<9; ++j) {
			 printf("%d", memory[i] [9-j]);
	} /* end word loop */
        printf("  ");
        ++i;
      } /* end column loop */
      printf("\n");
    } /* end row loop */
    printf("\n depress any key to continue");
    j = getchar();
  } /* end page loop */
}

void disp_mem_blk(int a)
{
  int  i, j;
//  clrscr();
  printf("\033[0K");
  gotoxy(20,1);
  printf("\nMemory Editor\n\n");
  gotoxy(15,5);
  printf("Loc: Data");
  gotoxy(15,6);
  printf("--------------------");
  for (j=0; j<10; ++j){
    gotoxy(15,7+j);
    printf("%03d:", a);
    for (i=1; i<9; ++i){
      gotoxy(18+2*i,7+j);
		printf("%d", memory [a] [9-i]);
    }
    ++a;
  }
  printf("\n\n\n (2,4,6,8) to move cursor, <space bar> to toggle bit,\n");
  printf(" n -next memory block, x -exit.\n");
}

void load_memory()                                           //Load Memory
{
  int  a, i, start_adr, bucket;
  a=256;
//  clrscr();
  printf("\033[0K");
  gotoxy(20,1);
  printf("Memory Editor\n");
  while ((a < 0) || (a > 246)){
	 printf("\n Enter starting address of memory block (0-246, decimal): ");
	 scanf("%d", &a);
  }
  start_adr=a;
  disp_mem_blk(a);
  a=0;
  i=0;
  while (choice != 120) {
	 gotoxy(20+i,7+a);
	 choice = getchar();
	 if (choice == 54) {             //right 6
      i=i+2;
      if (i > 14) i=0;
    }
	 if (choice == 52) {             //left 4
      i=i-2;
      if (i < 0) i=14;
    }
	 if (choice == 50) {             //down 2
      a=a+1;
      if (a > 9) a=0;
    }
	 if (choice == 56) {             //up 8
      a=a-1;
      if (a < 0) a=9;
    }
    if (choice == 32) {             //toggle bit
		bucket=memory [(start_adr+a)] [9-((i/2)+1)];
      if (bucket == 0) {
        bucket=1;
	printf("%d",bucket);
      }
      else if (bucket == 1) {
        bucket=0;
	printf("%d",bucket);
      }
		memory [(start_adr+a)] [9-((i/2)+1)]=bucket;
    }
    if (choice == 110) {            // next block
      start_adr=start_adr+10;
      if (start_adr > 246) start_adr=0;
      disp_mem_blk(start_adr);
	   gotoxy(20+i,7+a);
	 }
  }
}

void memory_mode()
{
  while (choice != 51) {
//    clrscr();
   printf("\033[0K");
	 printf("                    J8CM Memory Panel\n\n");
	 printf("     1. Edit contents of memory\n");
	 printf("     2. Display contents of memory\n\n");
	 printf("     3. Return to main menu\n");
	 choice = getchar();
    if (choice == 49) load_memory();
	 if (choice == 50) disp_mem_pg();
  }
  choice=0;
}                                        // end memory mode routines
