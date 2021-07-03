// j8cm_clr.cpp
// J8CM project
// cmsc 265

void clear_regs()
{
  int i;
  for (i=1; i<9; ++i)
	 accumulator[i]=zero;
  for (i=1; i<9; ++i)
	 xreg[i]=zero;
  for (i=1; i<9; ++i)
	 yreg[i]=zero;
  for (i=1; i<9; ++i)
    IR[i]=zero;
  for (i=1; i<9; ++i)
    PC[i]=zero;
  for (i=1; i<9; ++i)
    SP[i]=zero;
  for (i=1; i<9; ++i)
	 MAR[i]=zero;
  i_flag = -1;
  FLG.c_flag = 0;
  FLG.v_flag = 0;
  FLG.n_flag = 0;
  FLG.z_flag = 1;
}

void clear_mem()
{
  int i, j, k, l, m;
  i=0;
  for (m=1; m<5; ++m) {
	 for (l=1; l<17; ++l) {
		for (k=1; k<5; ++k) {
		  for (j=1; j<9; ++j) {
			 memory[i] [j] = zero;
	} /* end word loop */
	++i;
		} /* end column loop */
    } /* end row loop */
  } /* end page loop */
}

void clear_io_in()
{
  int i, j;
  for (i=0; i<iolen; ++i) {
    for (j=1; j<9; ++j) {
      io_in[i] [j] = zero;
    }
  }
}

void clear_io_out()
{
  int i, j;
  for (i=0; i<iolen; ++i) {
    for (j=1; j<9; ++j) {
      io_out[i] [j] = zero;
    }
  }
}

void init_bootstrap()
{
  int i, j;
  printf("\n      Initializing ROM ..\n");
  for (i=0; i<6; ++i) {              // zero first 6 memory locations
	 for (j=1; j<9; ++j) {
			 memory[i] [j] = zero;
	 } /* end bit loop */
  } /* end address loop */
  //                     location 0 -input instruction, 0's
  memory[1] [1] = 1; //  location 1 -input argument
  memory[1] [2] = 1; //              set contents 3
  //                     location 2 -input instruction, 0's
  //                     location 3 -input argument, first byte from I/O in.
  memory[4] [2] = 1;  // location 4 -jump instruction
  memory[4] [3] = 1;  //             set contents to 6
  //                     location 5 -argument for jump instruction
} // end init_ROM

void data_destruct()
{
  choice = 0;
  while (choice != 54) {
    system("clear");
    printf("               JEB 1 Data Destruct Menu\n\n");
	 printf(" 1. Clear memory\n");
    printf(" 2. Clear registers\n");
    printf(" 3. Clear io_in\n");
	 printf(" 4. Clear io_out\n");
	 printf(" 5. Initialize bootstrap\n");
	 printf(" 6. Return to main menu\n");
    choice = getchar();
    if (choice == 49) {
      printf(" Are you sure you wish to clear memory?(y/n)\n");
      choice = getchar();
      if (choice == 121) clear_mem();
    }
    if (choice == 50) clear_regs();
    if (choice == 51) {
      printf(" Are you sure you wish to clear io_in?(y/n)\n");
      choice = getchar();
      if (choice == 121) {
		  clear_io_in();
		  in_mrkr=0;
      }
    }
	 if (choice == 52) {
		clear_io_out();
      out_mrkr=0;
	 }
	 if (choice == 53) init_bootstrap();
  }
  choice = 0;
}
