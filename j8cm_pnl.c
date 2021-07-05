// j8cm_pnl.cpp
// panel mode source code
// Jay's 8-bit Computer Model
// Jay Babel
// cmsc265
// Spring 1995
// Updated 2021

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

						  // register display routines
void reg_border(int c, int r)
{
  gotoxy(c,r+1);
  printf("-----------------");
  gotoxy(c,r+3);
  printf("-----------------");
}

void print_pc(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("PC");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i, row+2);
	 printf("%d", PC[9-i]);
  }
}

void print_ir(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("IR");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i, row+2);
	 printf("%d", IR[9-i]);
  }
}

void print_sp(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("SP");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i, row+2);
	 printf("%d", SP[9-i]);
  }
}

void print_mar(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("MAR");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i,row+2);
	 printf("%d", MAR[9-i]);
  }
}

void print_psw(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("PSW");
  gotoxy(col,row+1);
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i,row+2);
	 printf("%d", PSW[9-i]);
  }
}

void print_acc(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("Accumulator");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i,row+2);
	 printf("%d", accumulator[9-i]);
  }
}

void print_xreg(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("X Register");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i,row+2);
	 printf("%d", xreg[9-i]);
  }
}

void print_yreg(int col, int row)
{
  int i;
  gotoxy(col,row);
  printf("Y Register");
  reg_border(col, row);
  for (i=1; i<9; ++i) {
	 gotoxy((col-1)+2*i,row+2);
	 printf("%d", yreg[9-i]);
  }
}

void disp_regs()
{
  print_pc(4,3);
  print_ir(26,3);
  print_sp(48,3);
  print_mar(4,8);
  print_psw(26,8);
  print_acc(4,13);
  print_xreg(4,18);
  print_yreg(26,18);
}
					 // end register display routines

void 	 i_flag_out(int col, int row)
{
  // fetch
  if (i_flag == 0) {
	 gotoxy(col,row);
	 printf("*");
	 gotoxy(col,row+1);
	 printf(" ");
	 gotoxy(1,25);
  }
  // execute
  if (i_flag == 1) {
	 gotoxy(col,row+1);
	 printf("*");
	 gotoxy(col,row);
	 printf(" ");
	 gotoxy(1,25);
  }
}

void cycle_indicator(int col, int row) // indicate fetch/execute cycle
{
  int i;

  i=0;
  gotoxy(col,row+i);
  printf("cycle");
  i++;
  gotoxy(col,row+i);
  printf("--------");
  i++;
  gotoxy(col+1,row+i);
  printf("fetch");
  i++;
  gotoxy(col+1,row+i);
  printf("execute");
}

// ##################################
// ##################################
//     ###    #####   #   #
//    #   #   #    #  #   #
//    #       #    #  #   #
//    #       #####   #   #
//    #       #       #   #
//    #       #       #   #
//    #   #   #       #   #
//     ###    #        ###
#include</home/jay/repos/j8cm/j8cm_cpu.c>
//#include<j8_cpu1a.cpp>
// ##################################
// ##################################

void edit_reg(int col, int row)
{
  int bucket;

  bit = 0;
  gotoxy(4,22);
//  clreol();
  printf("\033[0K");
  printf("(4,6) to move cursor, <spacebar> to toggle bit");
  gotoxy(4,23);
//  clreol();
  printf("\033[0K");
  printf("x -exit program counter edit");
  gotoxy(col,row);
  while (choice != 120) {
	 gotoxy(col+bit,row);
	 choice = getchar();
	 if (choice == 54) {             //right 6
		bit=bit+2;
		if (bit > 14) bit=0;
	 }
	 if (choice == 52) {             //left 4
		bit=bit-2;
		if (bit < 0) bit=14;
	 }
	 if (choice == 32) {             //toggle bit
		bucket=PC[9-((bit/2)+1)];
		if (bucket == 0) {
		  bucket=1;
		  printf("%d",bucket);
		}
		else if (bucket == 1) {
		  bucket=0;
		  printf("%d",bucket);
		}
		PC[9-((bit/2)+1)]=bucket;
	 }
  }
  gotoxy(4,22);
  printf(" p -set PC, f -fetch, e -execute, s -step, r -run, c -clear");
  gotoxy(4,23);
//  clreol();
  printf("\033[0K");
  printf("    x -exit front panel");
}

void panel_mode()
{
	 system("clear");
	 i_flag = -1;
	 gotoxy(29,1);
	 printf("J8CM Front Panel");
	 disp_regs();
	 cycle_indicator(56,18);
	 i_flag_out(56,20);;
	 gotoxy(4,22);
	 printf(" p -set PC, f -fetch, e -execute, s -step, r -run, c -clear");
	 gotoxy(4,23);
	 printf("    x -exit front panel");
  while (choice != 120) {
	 if (choice == 112)edit_reg(5,4);        // p -set PC
	 if (choice == 102) fetch();             // f -fetch
	 if (choice == 101) execute();           // e -execute
	 if (choice == 115) step();              // s -step
	 if (choice == 114) run();               // r -run
	 if (choice == 99) {                     // c -clear
		clear_regs();
		disp_regs();
	 }
	 gotoxy(3,24);
	 choice = getchar();
  }
}
