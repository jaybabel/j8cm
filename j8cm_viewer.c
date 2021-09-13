#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define addresses  256
#define wordlen      9

int choice,
    page = 0,
    memory[addresses] [wordlen],  // memory 8x256
    snapshot_read = 0;

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

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

void draw_panel()
{
  system("clear");
  gotoxy(29,1);
  printf("J8CM Memory Viewer");
  gotoxy(4,22);
  printf(" r - refresh snapshot,");
  gotoxy(30,22);
  printf(" d - display memory");
  gotoxy(4,23);
  printf(" x - exit memory viewer");
}

void mem_view()
{
    FILE *icycfp, *msnapfp;
    int a, b, m;
    char str[1];

  if(remove("./i_cycle") != 0) {
  	 gotoxy(60,1); printf("No i_cycle file.\n");
   } else {
     remove("./i_cycle");
     gotoxy(60,1); printf("Snapshot read.  \n");
     msnapfp = fopen("./memory_snapshot", "r");
     for (a=0; a<256; ++a) {
       for (b=1; b<9; ++b) {
         fgets(str, 2, msnapfp);
         m = atoi(str);
         memory[a] [9-b] = m;
       } /* end bit loop */
     } /* end address loop */
     fclose(msnapfp);
     snapshot_read = 1;
   }
}

void disp_mem_pg()
{
  int i, j, k, l, m;
  i=0;
  if (snapshot_read == 1) {
    for (m=1; m<5; ++m) {
      system("clear");
      printf("\033[0K");
      printf("\n****  Memory Snapshot View, Page %d  *****\n", m-1);
      printf("______________________________________________\n\n");
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
      printf("\n Press any key to continue");
      set_conio_terminal_mode();
        j = getchar();
      reset_terminal_mode();
    } /* end page loop */
  } else {
      gotoxy(5,5); printf("       No memory snapshot read");  }
  draw_panel();
}

void panel_mode()
{
   draw_panel();
   mem_view();                            // read memory snapshot into memory array
   disp_mem_pg();
     while (choice != 120) {              // x - exit
       if (choice == 100)disp_mem_pg();   // d - display memory array
       if (choice == 114)mem_view(); 	    // r - refresh snapshot
	     gotoxy(3,23);
       set_conio_terminal_mode();
          choice = getchar();
       reset_terminal_mode();
     }
  system("clear");
  choice = 0;
}

int main(void)
{
//  mem_view();
  panel_mode();
  return(0);
}
