// 2/3/95
// j8cm.cpp
// Jay's 8-bit Computer Model
// Jay Babel
// cmsc265
// Spring 1995

//#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <math.h>

#define addresses  256
#define wordlen      9
#define iolen      200
#define zero         0
#define PATHLEN     44
#define RELNUM  2.0

int bit,
	 i_flag,                       // cycle indicator flag,
											 //    fetch sets to 0, execute to 1
	 choice,
	 in_mrkr=0,                    // i/o marker
	 out_mrkr=0,                   // i/o marker
	 accumulator[wordlen],
	 xreg[wordlen],                // general purpose register x
	 yreg[wordlen],                // general purpose register y
	 IR[wordlen],                  // instruction register
	 PC[wordlen],                  // program counter
	 MAR[wordlen],                 // memory address register
	 SP[wordlen],                  // stack pointer
	 PSW[wordlen],                 // processor status word
	 memory[addresses] [wordlen],  // memory 8x256
	 io_in[iolen] [wordlen],       // I/O 8x200
	 io_out[iolen] [wordlen];



struct word_node { int temp_word[wordlen]; };

struct i_node { int opcode,
						  adr_mode,
						  reg;
				  };

struct flg_node { int c_flag,       // carry flag
							 v_flag,       // overflow flag
							 n_flag,       // result negative
							 z_flag;       // result = 0
					 };

struct flg_node FLG;                // Processor Flags

// 2s_comp.cpp
struct word_node twos_comp(int d)
{
  int i, j, a_d, d_sum, b_sum[3];
  struct word_node r;

	 if (d < 0) {                     // two's comp.
		// absolute value
		a_d = abs(d);
		for (i=1; i<9; ++i){
		  r.temp_word[i] = a_d%2;
		  a_d=a_d/2;
		}
		// flip bits
		for (i=1; i<9; ++i){
		  if (r.temp_word[i] == 0) r.temp_word[i] = 1;
			 else r.temp_word[i] = 0;
		}
		// binary add 1
		b_sum[2] = 1;
		for (i=1; i<9; ++i){
		  d_sum = r.temp_word[i] + b_sum[2];
		  for (j=1; j<3; ++j) {
			 b_sum[j] = d_sum%2;
			 d_sum = d_sum/2;
		  }
		  r.temp_word[i] = b_sum[1];
		}
	 }
	 else {
		  for (i=1; i<9; ++i){
			 r.temp_word[i] = d%2;
			 d=d/2;
		  }
	 }
  return(r);
}

#include </home/jay/repos/j8cm/c/j8cm_clr.c>
#include </home/jay/repos/j8cm/c/j8cm_pnl.c>
// #include <j8cm_io.c>
// #include <j8cm_mem.c>
// #include <j8cm_ldr.c>

int main(void)
{
	 char menu_selection, confirm;
//  clear_mem();
//  clear_regs();
//  clear_io_in();
//  clear_io_out();
//  init_bootstrap();
//  while (choice != 121) {
	 system("clear");
	 do{
	 printf("          Jay's 8-bit Computer Model v%1.1f\n", RELNUM);

	 printf("                     Main Menu           \n\n");
	 printf("       1. Front Panel\n");
	 printf("       2. Memory Panel\n");
	 printf("       3. I/O Panel\n\n");
	 printf("       4. Loader\n\n");
	 printf("       5. Data Destruct\n");
	 printf("       6. Quit\n");
   scanf("%c",&menu_selection);

	 switch(menu_selection){

		case '1':
		     printf("\n Enter the Front Panel mode.\n\n");
				 break;
		case '2':
		     printf("\n Enter the Memory Panel mode.\n\n");
				 break;
		case '3':
		     printf("\n Enter the I/O Panel mode.\n\n");
				 break;
		case '4':
 			   printf("\n Enter the Loader.\n\n");
 				 break;
		case '5':
				 printf("\n Data Destruct.\n\n");
				 break;
		case '6':
	    	 printf(" Are you sure you wish to quit?(y/n)\n");
				 break;
		default:
		     printf("%c\n", menu_selection);
		     printf("invalid input\n");
				     break;
	 }
 } while(menu_selection !='6');
//  system("clear");
  printf("\n\n  end.");
  return 0;
}
