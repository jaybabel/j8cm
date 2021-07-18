// j8cm_io.cpp
// I/O mode source code
// Jay's Electronic Brain Model
// Jay Babel
// cmsc265
// Spring 1995


					 // I/O mode routines
void disp_io_in(int a)
{
  int i, j;
  for (i=0; i<10; ++i) {
	  for (j=1; j<9; ++j) {
		  gotoxy(11+2*j,5+i);
		  printf("%d", io_in[a] [9-j]);
	  }
	 ++a;
  }
  if ((in_mrkr >= (a-10)) && (in_mrkr < a)) {
	  gotoxy(7,5+in_mrkr%10);
	  printf("[%03d]>", in_mrkr);
  }
  else {
	  gotoxy(7,5+in_mrkr%10);
	  printf("      ");
  }
  gotoxy(1,16);
}

int io_in_decode(int l)                       // I/O in decode
{
  int a, i, borrow, t_io_in[wordlen];
  for (i=1; i<9; ++i)
	 t_io_in[i] = io_in[l] [i];
  a=0;
  if (t_io_in[8] == 1) {               // number is negative
	 // binary sub 1 from I/O out
	 i = 1;
	 borrow = 1;
	 while( borrow == 1) {
		if (t_io_in[i] == 1) {
		  t_io_in[i] = 0;
		  borrow = 0;
		}
		else {
		  t_io_in[i] = 1;
		  i++;
		}
	 }              // end binary sub
	 // flip bits
	 for (i=1; i<9; ++i){
		if (t_io_in[i] == 0) t_io_in[i] = 1;
		  else t_io_in[i] = 0;
	 }
	 for (i=1; i<8; ++i)
		a=a+t_io_in[i]*pow(2,(i-1));
	 if (a == 0) a = 128;
	 return -a;
  }
  else {     // number is positive
	 for (i=1; i<8; ++i)
		a=a+t_io_in[i]*pow(2,(i-1));
	 return a;
  }
}

void disp_in_dec(int a)
{
  int i, dec_in;
  for (i=0; i<10; ++i) {
	 gotoxy(13,5+i);
	 dec_in = io_in_decode(a);
	 printf("%d", dec_in);
	 ++a;
  }
  if ((in_mrkr >= (a-10)) && (in_mrkr < a)) {
	 gotoxy(7,5+in_mrkr%10);
	 printf("[%03d]>", in_mrkr);
  }
  else {
		gotoxy(7,5+in_mrkr%10);
		printf("      ");
  }
  gotoxy(1,16);
}

void disp_io_out(int a)
{
  int i, j;
  for (i=0; i<10; ++i) {
	  for (j=1; j<9; ++j) {
		  gotoxy(35+2*j,5+i);
		  printf("%d", io_out[a] [9-j]);
	  }
    ++a;
  }
  if ((out_mrkr >= (a-10)) && (out_mrkr < a)) {
	  gotoxy(31,5+out_mrkr%10);
	  printf("[%03d]>", out_mrkr);
  }
  else {
		gotoxy(31,5+out_mrkr%10);
		printf("      ");
  }
  gotoxy(1,16);
}  //end disp_io_out

void wipe_io_dsp()
{
  int i;
  for (i=5; i<16; ++i) {
	  gotoxy(8,i);
    printf("%c[2K", 27);  // clears to eol
  }
}

int io_out_decode(int l)                       // I/O out decode
{
  int a, i, borrow, t_io_out[wordlen];
  for (i=1; i<9; ++i)
	 t_io_out[i] = io_out[l] [i];
  a=0;
  if (t_io_out[8] == 1) {               // number is negative
	 // binary sub 1 from I/O out
	  i = 1;
	  borrow = 1;
	  while( borrow == 1) {
		  if (t_io_out[i] == 1) {
		    t_io_out[i] = 0;
		    borrow = 0;
		  }
		  else {
		    t_io_out[i] = 1;
		    i++;
		  }
	 }              // end binary sub
	 // flip bits
	 for (i=1; i<9; ++i){
		if (t_io_out[i] == 0) t_io_out[i] = 1;
		  else t_io_out[i] = 0;
	 }
	 for (i=1; i<8; ++i)
		a=a+t_io_out[i]*pow(2,(i-1));
	 if (a == 0) a = 128;
	 return -a;
  }
  else {     // number is positive
	 for (i=1; i<8; ++i)
		a=a+t_io_out[i]*pow(2,(i-1));
	 return a;
  }
}  // end io_out_decode

void disp_out_dec(int a)
{
  int i, dec_out;
  for (i=0; i<10; ++i) {
	  gotoxy(37,5+i);
	  dec_out = io_out_decode(a);
	  printf("%d", dec_out);
	  ++a;
  }
  if ((out_mrkr >= (a-10)) && (out_mrkr < a)) {
	  gotoxy(31,5+out_mrkr%10);
	  printf("[%03d]>", out_mrkr);
  }
  else {
		gotoxy(31,5+out_mrkr%10);
		printf("      ");
  }
  gotoxy(1,16);
} // end disp_out_dec

void print_blk_count(int s_a)
{
  gotoxy(22,3);
  printf("blk %02d", s_a/10);
  gotoxy(46,3);
  printf("blk %02d", s_a/10);
}

void io_silk_screen()
{
  system("clear");
  gotoxy(24,1);
  printf("J8CM I/O Panel\n");
  gotoxy(7,3);
  printf("I/O In");
  gotoxy(7,4);
  printf("---------------------");
  gotoxy(31,3);
  printf("I/O Out");
  gotoxy(31,4);
  printf("---------------------");
  gotoxy(1,20);
  printf("  (2,4,6,8) to move cursor, <space bar> to toggle bit, ");
  printf("c -clear I/O out");
  gotoxy(1,21);
  printf("       m -I/O marker reset, x -exit, n -next I/O block, b -base 10");
}

void io_mode()
{
  int  a, i, bucket, start_adr, b_toggle;
  a=256;
  b_toggle = 1;
  start_adr = 0;
  io_silk_screen();
  print_blk_count(start_adr);
  disp_io_in(start_adr);
  disp_io_out(start_adr);
  a=0;
  i=0;
  while (choice != 120) {
	  gotoxy(13+i,5+a);
    set_conio_terminal_mode();
      choice = getchar();
    reset_terminal_mode();
	  if (b_toggle == 1) {
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
      if (choice == 99) {             //clear io_out
        clear_io_out();
		    gotoxy(31,5+out_mrkr%10);
		    printf("      ");
		    out_mrkr=0;
		    disp_io_out(start_adr);
	    }
	    if (choice == 32) {             //toggle bit
		    bucket=io_in [start_adr+a] [9-((i/2)+1)];
		    if (bucket == 0) {
		      bucket=1;
		      printf("%d",bucket);
		    }
		    else if (bucket == 1) {
		      bucket=0;
		      printf("%d",bucket);
		    }
		    io_in [start_adr+a] [9-((i/2)+1)]=bucket;
	    }
	 } // end if (b_toggle == 1)
	 if (choice == 109) {             //reset I/O markers
	 	 wipe_io_dsp();
		 start_adr = 0;
		 disp_io_in(start_adr);
		 disp_io_out(start_adr);
		 print_blk_count(start_adr);
		 gotoxy(7,5+in_mrkr%10);
		 printf("      ");
		 in_mrkr=0;
		 gotoxy(7,5+in_mrkr%10);
		 printf("[%03d]>", in_mrkr);
		 gotoxy(31,5+out_mrkr%10);
		 printf("      ");
		 out_mrkr=0;
		 gotoxy(31,5+out_mrkr%10);
		 printf("[%03d]>", out_mrkr);
		 gotoxy(1,16);
	 }                             // end reset I/O markers
	 if (choice == 110) {            // next block
		 start_adr=start_adr+10;
		 if (start_adr > 190) start_adr=0;
		 print_blk_count(start_adr);
		 if (b_toggle == 0) {
		   wipe_io_dsp();
		   disp_in_dec(start_adr);
		   disp_out_dec(start_adr);
		 }
		 if (b_toggle == 1) {
		   wipe_io_dsp();
		   disp_io_in(start_adr);
		   disp_io_out(start_adr);
		 }
		 gotoxy(20+i,7+a);
	 }
	 if (choice == 98) {                // b toggle base
		 if (b_toggle == 1) {             // base 10
		   wipe_io_dsp();
		   disp_in_dec(start_adr);
//		  disp_out_dec((out_mrkr/10)*10);
		   disp_out_dec(start_adr);
		   gotoxy(1,20);
//		  clreol();
		   gotoxy(65,21);
		   printf("2 ");
		   b_toggle = 0;
		 }
		 else if (b_toggle == 0) {        // base 2
		   wipe_io_dsp();
		   disp_io_in(start_adr);
//		  disp_io_out((out_mrkr/10)*10);
		   disp_io_out(start_adr);
		   gotoxy(1,20);
		   printf("(2,4,6,8) to move cursor, <space bar> to toggle bit, c -clear I/O out");
		   gotoxy(65,21);
		   printf("10");
		   b_toggle = 1;
		 }
	 } // end toggle base
  }
  system("clear");
  choice = 0;
}
							// end I/O mode routines
