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
#include <math.h>
#include <termios.h>
#include <unistd.h>


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
	 memory[addresses] [wordlen],  // memory 256x8
	 io_in[iolen] [wordlen],       // I/O 200x8
	 io_out[iolen] [wordlen];

	 struct termios orig_termios;

	 void reset_terminal_mode()
	 {
	     tcsetattr(0, TCSANOW, &orig_termios);
	 }

	 void set_conio_terminal_mode()
	 {
	     struct termios new_termios;

	     /* take two copies - one for now, one for later */
	     tcgetattr(0, &orig_termios);
	     memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	     /* register cleanup handler, and set the new terminal mode */
	     atexit(reset_terminal_mode);
	     cfmakeraw(&new_termios);
	     tcsetattr(0, TCSANOW, &new_termios);
	 }

	 int getch()
	 {
	     int r;
	     unsigned char c;
	     if ((r = read(0, &c, sizeof(c))) < 0) {
	         return r;
	     } else {
	         return c;
	     }
	 }

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

#include </home/jay/repos/j8cm/j8cm_clr.c>
#include </home/jay/repos/j8cm/j8cm_pnl.c>
#include </home/jay/repos/j8cm/j8cm_mem.c>
#include </home/jay/repos/j8cm/j8cm_io.c>
#include </home/jay/repos/j8cm/j8cm_ldr.c>
char menu_selection, confirm;

int main(void)
{
system("clear");
	 do{
//	 char menu_selection, confirm;
	 printf("          Jay's 8-bit Computer Model v%1.1f\n", RELNUM);

	 printf("                     Main Menu           \n\n");
	 printf("       1. Front Panel\n");
	 printf("       2. Memory Menu\n");
	 printf("       3. I/O Panel\n\n");
	 printf("       4. Loader\n\n");
	 printf("       5. Data Destruct\n");
	 printf("       6. Quit\n");
//   scanf("%c",&menu_selection);
   set_conio_terminal_mode();
	   menu_selection = getchar();
   reset_terminal_mode();

	 switch(menu_selection){

		case '1':
		     system("clear");
				 panel_mode();
				 break;
		case '2':
				 memory_mode();
				 break;
		case '3':
				 io_mode();
				 break;
		case '4':
				 loader();
 				 break;
		case '5':
				 data_destruct();
				 snapshot();
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
