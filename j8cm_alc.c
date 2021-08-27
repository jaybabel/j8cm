// j8cm_alc.cpp
// Jay's 8-bit Computer Model Assembly Language Compiler
// Jay Babel
// February 1995


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define WORDLEN 47
#define CODELEN 25
#define PATHLEN 50
#define RELNUM  2.0
// Supports memory direct, memory indirect, register direct and
// register indirect addressing modes.

// Added non-standard itoa routine found on stackoverflow original name my_itoa
#define INT_LEN (10)
#define HEX_LEN (8)
#define BIN_LEN (32)
#define OCT_LEN (11)

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

static char *  itoa ( int value, char * str, int base )
{
    int i,n =2,tmp;
    char buf[BIN_LEN+1];


    switch(base)
    {
        case 16:
            for(i = 0;i<HEX_LEN;++i)
            {
                if(value/base>0)
                {
                    n++;
                }
            }
            snprintf(str, n, "%x" ,value);
            break;
        case 10:
            for(i = 0;i<INT_LEN;++i)
            {
                if(value/base>0)
                {
                    n++;
                }
            }
            snprintf(str, n, "%d" ,value);
            break;
        case 8:
            for(i = 0;i<OCT_LEN;++i)
            {
                if(value/base>0)
                {
                    n++;
                }
            }
            snprintf(str, n, "%o" ,value);
            break;
        case 2:
            for(i = 0,tmp = value;i<BIN_LEN;++i)
            {
                if(tmp/base>0)
                {
                    n++;
                }
                tmp/=base;
            }
            for(i = 1 ,tmp = value; i<n;++i)
            {
                if(tmp%2 != 0)
                {
                    buf[n-i-1] ='1';
                }
                else
                {
                    buf[n-i-1] ='0';
                }
                tmp/=base;
            }
            buf[n-1] = '\0';
            strcpy(str,buf);
            break;
        default:
            return NULL;
    }
    return str;
}  // end stackoverflow code

// another stackoverflow solution
char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}
//  ---------------------------------------

int obj_arg, obj_ins, xchoice, adrctr, errcode;

struct st_node                            // symbol tree node
  {  char key[WORDLEN];
	  int value;
	  struct st_node *l, *r; };

struct st_node *st_t, *st_head, *st_z;

struct rhs_node                // right hand side node for instruction node
  { int ml_code,
		  num_args; } *rhs_size;

struct inst_node                        // binary search tree inst_node
  {  char op_code[WORDLEN];
	  struct rhs_node rhs;
	  struct inst_node *l, *r; };

struct inst_node *inst_t, *inst_head, *inst_z;

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void symboltreeinit()
  {
	 st_z = (struct st_node *) malloc(sizeof *st_z);
	 st_z->l = st_z; st_z->r = st_z; st_z->value = -1;
	 st_head = (struct st_node *) malloc(sizeof *st_head);
	 st_head->r = st_z; strcpy(st_head->key,"");
  }

void inst_treeinit()
  {
	 rhs_size = (struct rhs_node *) malloc(sizeof *rhs_size);
	 inst_z = (struct inst_node *) malloc(sizeof *inst_z);
	 inst_z->l = inst_z; inst_z->r = inst_z;
	 inst_z->rhs.ml_code = -1; inst_z->rhs.num_args = -1;
	 inst_head = (struct inst_node *) malloc(sizeof *inst_head);
	 inst_head->r = inst_z; strcpy(inst_head->op_code,"");
}

void symboltreeinsert(char k[WORDLEN], int v)
  {
	 struct st_node *parent, *x;
	 parent = st_head; x = st_head->r;
	 while (x != st_z)
		{ parent = x; x = (0 > strcmp(k, x->key)) ? x->l : x->r; }
	 x = (struct st_node *) malloc(sizeof *x);
	 strncpy(x->key, k, WORDLEN); x->value = v;
	 x->l = st_z; x->r = st_z;
	 if (0 > strcmp(k, parent->key)) parent->l = x; else parent->r = x;
  }

void inst_treeinsert(char lhs[WORDLEN], int m_code, int n_args)
  {
	 struct inst_node *parent, *x;
	 parent = inst_head; x = inst_head->r;
	 while (x != inst_z)
		{ parent = x; x = (0 > strcmp(lhs, x->op_code)) ? x->l : x->r; }
	 x = (struct inst_node *) malloc(sizeof *x);
	 strncpy(x->op_code, lhs, WORDLEN);
	 x->rhs.ml_code = m_code; x->rhs.num_args = n_args;
	 x->l = inst_z; x->r = inst_z;
	 if (0 > strcmp(lhs, parent->op_code)) parent->l = x; else parent->r = x;
  }

void read_inst()
{
  FILE *infp;
  int x;
  int i;
  char op[WORDLEN];
system("clear");
  printf("\n READING Instruction Set\n");
  if((infp=fopen("/home/jay/repos/j8cm_local/xinstr.set", "r")) == NULL) {
	 printf("cannot open file\n");
  }
  while (!feof(infp)) {
	 fscanf(infp, "%s%d%d\n", op, &xchoice, &x);
	 inst_treeinsert(op, xchoice, x);
//	 strset(op,' ');
   memset(op,0,WORDLEN);
  }
  fclose(infp);
}

// start symbol tree write routines
void wrt_st_node(FILE *fp, struct st_node *x)
  {
	 fprintf(fp, "%s %d \n", x->key, x->value);
  }

void sym_tree_wrtr(FILE *fp, struct st_node *x)
  {
	 if (x != st_z) {
		sym_tree_wrtr(fp, x->l);
		wrt_st_node(fp, x);
		sym_tree_wrtr(fp, x->r);
	 }
  }

void write_symbol(char *fnm)
{
  FILE *outfp;
  char fnm_x[70];

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".sym");
  gotoxy(5,8); printf("Saving symbol tree as %s", fnm_x);
  if((outfp=fopen(fnm_x, "w")) == NULL) {
	 printf("cannot open file\n");
  }
  sym_tree_wrtr(outfp, st_head->r);
  fclose(outfp);
  gotoxy(5,10); printf("Symbol tree saved as %s ", fnm_x);
}

void print_st_node(struct st_node *x)
  {
	 printf("%s", x->key);
	 printf("     %02d\n", x->value);
  }

void symtreeprintr(struct st_node *x)
  {
	 if (x != st_z) {
		symtreeprintr(x->l);
		print_st_node(x);
		symtreeprintr(x->r);
	 }
  }

void symtreeprint()
{
  printf("Symbol Table\n\n");
  symtreeprintr(st_head->r);
}

int symbolsearch(char lhs[WORDLEN])
{
  struct st_node *x;
  int rhs;

  rhs = -1;
  x = st_head->r;
  while (x != st_z){
	 if (0 == strncmp(lhs, x->key, strlen(x->key))){
		rhs = x->value;
		x = st_z;
	 }
	else x = (0 > strncmp(lhs, x->key, strlen(x->key))) ? x->l : x->r;
  }
  return(rhs);
}

struct rhs_node inst_treesearch(char lhs[WORDLEN])
{
  struct inst_node *x;
  struct rhs_node r;

  r.ml_code = -1; r.num_args = -1;
  x = inst_head->r;
  while (x != inst_z){
	 if (0 == strncmp(lhs, x->op_code, strlen(x->op_code))){
		r.ml_code = x->rhs.ml_code;
		r.num_args = x->rhs.num_args;
		x = inst_z;
	 }
	else x = (0 > strncmp(lhs, x->op_code, strlen(x->op_code))) ? x->l : x->r;
  }
  return(r);
}

void screen_out(int x, int r, int ac, int i, int a,
					 char l[WORDLEN], char mi[WORDLEN], char ma[WORDLEN])
{
  char bin_ac[WORDLEN], bin_i[WORDLEN], bin_a[WORDLEN];

  if (x == 4) {
	 itoa(ac, bin_ac, 2); itoa(i, bin_i, 2); itoa(a, bin_a, 2);
	 ac=atoi(bin_ac); i=atoi(bin_i);
	 gotoxy(2, r); printf("%08d %08d         ", ac, i);
  }
  if (x < 2) {
	 itoa(ac, bin_ac, 2); itoa(i, bin_i, 2); itoa(a, bin_a, 2);
	 ac=atoi(bin_ac); i=atoi(bin_i); a=atoi(bin_a);
	 gotoxy(2, r); printf("%08d %08d %08d", ac, i, a);
  }
  if (x < 3) {
	 gotoxy(33, r); printf("%s", l);
  }
  gotoxy(41, r); printf("%s", mi);
  gotoxy(49, r); printf("%s", ma);
}

void make_ldr(char *fnm, int jump_start)
{
  FILE *infp, *outfp;
  char fnm_x[70], ofnm_x[70];
  int adr_out, i_out;

  strcpy(fnm_x, fnm); strcat(fnm_x, ".obj");
  strcpy(ofnm_x, fnm); strcat(ofnm_x, ".ldr");
  gotoxy(5,11); printf("Creating %s",ofnm_x);
  if ((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open file\n");
  }
  if((outfp=fopen(ofnm_x, "w")) == NULL) {
	 printf("cannot open file\n");
  }
  fscanf(infp, "%d%d", &adrctr, &obj_ins);
  while (!feof(infp)) {
	 adr_out = adrctr; i_out = obj_ins;
	 fprintf(outfp, "%d\n", adr_out);
	 fprintf(outfp, "%d\n", i_out);
	 fscanf(infp, "%d%d", &adrctr, &obj_ins);
  }
  fclose(infp);
  i_out = 5; fprintf(outfp, "%d\n", i_out);           // append input
  i_out = jump_start; fprintf(outfp, "%d\n", i_out);  //  & jump arguments
  fclose(outfp);
}                                                       // end make_ldr

void pass2(char *fnm)          //------------- Pass 2 -----------
{
  FILE *infp, *outfp, *sumfp;
  struct rhs_node rhs_temp;
  char src_code[WORDLEN], symbol[WORDLEN], src_op[WORDLEN],
		 src_arg[WORDLEN], label[WORDLEN], fnm_x[70], ofnm_x[70], sfnm_x[70];
  int row=2, inst1_adr, len, i;

  strcpy(fnm_x, fnm); strcat(fnm_x, ".asm"); strcpy(ofnm_x, fnm);
  strcat(ofnm_x, ".obj"); strcpy(sfnm_x, fnm); strcat(sfnm_x, ".sum");
  memset(symbol,0,strlen(symbol));
  if ((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open file\n");
  }
  if((outfp=fopen(ofnm_x, "w")) == NULL) {
	 printf("cannot open file\n");
  }
  if((sumfp=fopen(sfnm_x, "w")) == NULL) {
	 printf("cannot open file\n");
  }
  row++;
  while (!feof(infp)) {
	 fscanf(infp, "%s", src_code);
	 if (strstr(src_code, "*") != NULL){  // ignore remark statements
   memset(src_code,0,strlen(src_code));
		while (strstr(src_code, "*") == NULL) {
		  fscanf(infp, "%s", src_code);
		}
	 }
	 if (strstr(src_code, ":") != NULL){            // src_code is a label
		strcpy(label, src_code);
	 }
	 else {                                         // src_code is not a label
		strncpy(symbol, src_code, strlen(src_code));
		if(strcmp(src_code, "ORG") == 0){
		  strcpy(src_op, src_code);
		  fscanf(infp, "%s", src_code);
		  strcpy(src_arg, src_code);
		  adrctr = atoi(src_code);
		  row++;
		  fprintf(sumfp, "3 %d %s %s\n", row, src_op, src_arg);
		  row++;
		}
		else if(strcmp(src_code, "END") == 0){
		  strcpy(src_op, src_code);
		  fscanf(infp, "%s", src_code);
		  strcpy(src_arg, src_code);
		  obj_arg = symbolsearch(src_code);
		  inst1_adr = obj_arg;
		  fprintf(sumfp, "3 %d %s %s\n", row, src_op, src_arg);
		  row++;
		}
		else if(strcmp(src_code, "DAT") == 0){
		  strcpy(src_op, src_code);
		  fscanf(infp, "%s", src_code);
		  strcpy(src_arg, src_code);
		  obj_ins=0;obj_arg=atoi(src_arg);
		  fprintf(outfp, "%d %d\n", adrctr, obj_arg);
		  fprintf(sumfp, "1 %d %d %d %d %s %s %s\n", row, adrctr, obj_ins, obj_arg, label,
					 src_op, src_arg);
		  row++; adrctr++;
      memset(label,0,strlen(label));
		}
		else if(strcmp(src_code, "EQU") == 0){
		  strcpy(src_op, src_code);
		  fscanf(infp, "%s", src_code);
		  strcpy(src_arg, src_code);
		  fprintf(sumfp, "2 %d %s %s %s\n", row, label, src_op, src_arg);
		  row++;
      memset(label,0,strlen(label));
		 }
	  else {                                             // instruction
		 rhs_temp = inst_treesearch(symbol);
		 obj_ins = rhs_temp.ml_code;
		 if ((obj_ins > -1)&&(rhs_temp.num_args == 1)) { // inst. with arg.
			strcpy(src_op, symbol);
      memset(symbol,0,strlen(symbol)); memset(src_arg,0,strlen(src_arg));
			fscanf(infp, "%s", src_code);                 // read operand
			strcpy(src_arg, src_code);         // <-src_arg for summary file
      memset(symbol,0,strlen(symbol)); strcpy(symbol, src_code);
			// ====================== address modes =====================
			if (strstr(src_code, "(") != NULL) {                     // indirect
        strcpy(symbol,&src_code[1]);                           // removes ( from symbol
        memset(src_code,0,strlen(src_code));
        strncpy(src_code,symbol,strlen(symbol)-1);             // copies symbol without ( ) to src_code
        memset(symbol,0,strlen(symbol));
			  strcpy(symbol,src_code);                              // copies src_code without ) to symbol
			  obj_ins = obj_ins + 32;                               //sets bit 32 of the instruction - indirect addressing
			}
			if (strstr(src_code, "/X") != NULL) {    // X register direct
			  obj_ins = obj_ins + 64;
			  rhs_temp.num_args=0;
			}
			if (strstr(src_code, "/Y") != NULL) {    // Y register direct
			  obj_ins = obj_ins + 192;
			  rhs_temp.num_args=0;
			}
			// ==========================================================
			if (strstr(src_code, "/") == NULL) {
			  obj_arg = symbolsearch(symbol);
			  if (obj_arg == -1) obj_arg=atoi(symbol);
			}
			fprintf(outfp, "%d %d\n", adrctr, obj_ins);
			if (rhs_temp.num_args == 1) {
			  fprintf(outfp, "%d %d\n", adrctr+1, obj_arg);
			  fprintf(sumfp, "1 %d %d %d %d %s %s %s\n", row, adrctr, obj_ins,
					  obj_arg, label, src_op, src_arg);
			}
			else
			  fprintf(sumfp, "1 %d %d %d %s %s %s\n", row, adrctr, obj_ins,
						 label, src_op, src_arg);
			row++; adrctr = adrctr+1+rhs_temp.num_args;
//			strset(label, ' ');
      memset(label,0,strlen(label));
		 }                                        // end if -inst. with arg.
		 else if (obj_ins > -1) {                 // inst. with no arg.
//			strset(src_arg,' ');
      memset(src_arg,0,strlen(src_arg));
			strcpy(src_op, symbol);
//			strset(symbol,' ');
      memset(symbol,0,strlen(symbol));
			fprintf(outfp, "%d %d\n", adrctr, obj_ins);
			fprintf(sumfp, "4 %d %d %d %s %s\n", row, adrctr, obj_ins,
					  label, src_op);
			row++;
			adrctr = adrctr+1+rhs_temp.num_args;
		 }                                       // end inst. with no arg.
	  }                                         // end else is instruction
	 }                                          // end else not label
//	 strset(symbol,' ');
  memset(symbol,0,strlen(symbol));
  }                                            // end while !eof
  fclose(infp); fclose(outfp); fclose(sumfp);
  make_ldr(fnm, inst1_adr);
  gotoxy(5,14); printf("Executable file written.");
} // end pass2

void pass1err(int ecode)
{
  if (ecode == 1)
	 printf("\npass 1 error - invalid label name. (instruction mnemonic)");
  if (ecode == 2) printf("\npass 1 error - missing END statement or argument");
  if (ecode == 3) printf("\npass 1 error - duplicate label");
  if (ecode == 4) printf("\npass 1 error - invalid argument");
  if (ecode == 5) printf("\npass 1 error - invalid END argument");
  if (ecode == 6) printf("\npass 1 error - invalid label. (length)");
  if (ecode == 7)
	 printf("\npass 1 error - invalid label name. (p_instruction mnemonic)");
  if (ecode > 7) printf("\npass 1 error - ");
}

int pass1(char *fnm)                       //------------- Pass 1 -----------
{
  FILE *infp, *errlogfp;
  struct rhs_node rhs_temp;
  int len, value, errchk, pass_enable, success=1;
  char src_code[WORDLEN], symbol[WORDLEN], fnm_x[70];

  strcpy(fnm_x, fnm); strcat(fnm_x, ".asm");
  if((errlogfp=fopen("/home/jay/repos/j8cm_local/error.log", "w")) == NULL) {
	 gotoxy(5,5); printf("Pass 1 - error - cannot open error log\n");
  }
  if ((infp=fopen(fnm_x, "r")) == NULL) {
	 gotoxy(5,5); printf("Pass 1 - error - cannot open source file\n");
	 success=0; return(success);
  }
  pass_enable = 1;
  while (pass_enable == 1) {
	 fscanf(infp, "%s", src_code);
	 if (feof(infp)) {
		fprintf(errlogfp, "%d\n", 2); // missing END statement or argument
		success=0; pass_enable = 0;
	 }
	 // ignore remark statements, trap for missing *
	 if (strstr(src_code, "*") != NULL){
//		strset(src_code,' ');
    memset(src_code,0,strlen(src_code));
		while ((strstr(src_code, "*") == NULL)&& (!feof(infp))) {
		  fscanf(infp, "%s", src_code);
		}
	 }
	 if (strstr(src_code, ":") != NULL) {   // src_code is a label
		len = strcspn(src_code, ":");
		if (len > 5) {
		  fprintf(errlogfp, "%d\n", 6); // invalid label - length violation
		  success=0; len = 5;
		}
//		strset(symbol, ' ');
    memset(symbol,0,strlen(symbol));
		strncpy(symbol, src_code, len);
		rhs_temp = inst_treesearch(symbol);
		errchk = rhs_temp.ml_code;
		if (errchk > -1) {                      // invalid label - inst
		  fprintf(errlogfp, "%d\n", 1); success=0;
		}
		if(strncmp(symbol, "ORG", len) == 0){   // invalid label - pinst
		  fprintf(errlogfp, "%d\n", 7); success=0;
		}
		if(strncmp(symbol, "EQU", len) == 0){   // invalid label - pinst
		  fprintf(errlogfp, "%d\n", 7); success=0;
		}
		if(strncmp(symbol, "DAT", len) == 0){   // invalid label - pinst
		  fprintf(errlogfp, "%d\n", 7); success=0;
		}
		errchk = symbolsearch(symbol);
		if (errchk > -1) {                       // duplicate label
		  fprintf(errlogfp, "%d\n", 3); success=0;
		}
		fscanf(infp, "%s", src_code);
		if(strcmp(src_code, "EQU") == 0){     // insert constant into symbol table
		  fscanf(infp, "%s", src_code); value = atoi(src_code);
		  symboltreeinsert(symbol, value);
		}
		else {
		  value = adrctr;
		  if(strcmp(src_code, "DAT") == 0) adrctr++;
//        else if(strcmp(src_code, "ASL") == 0) adrctr+5;
			    else adrctr = adrctr+2;
		  symboltreeinsert(symbol, value); // insert label into symbol table
		  fscanf(infp, "%s", src_code);
		}
	 }
	 else if (strstr(src_code, ":") == NULL){
		if(strcmp(src_code, "ORG") == 0){
		  fscanf(infp, "%s", src_code);
		  adrctr = atoi(src_code);
		}
		if(strcmp(src_code, "END") == 0){
		  fscanf(infp, "%s", src_code);
		  len = strlen(src_code);
//		  strset(symbol, ' ');
      memset(symbol,0,strlen(symbol));
		  strncpy(symbol, src_code, len);
		  errchk = symbolsearch(symbol);
		  if (errchk == -1) {
			 fprintf(errlogfp, "%d\n", 5); // invalid END argument
			 success=0;
		  }
		  if (!feof(infp)) pass_enable = 0;
		}
		rhs_temp = inst_treesearch(src_code);
		if (rhs_temp.ml_code > -1){
		  if (rhs_temp.num_args == 1) {
			 fscanf(infp, "%s", src_code);               // read inst. argument
			 if (strstr(src_code, "/") != NULL) rhs_temp.num_args = 0;
       // added ASL check didn't work
         // if (strstr(src_code, "ASL") != NULL) rhs_temp.num_args = 0;
			 adrctr = adrctr+1+rhs_temp.num_args;
			 rhs_temp = inst_treesearch(src_code);       // error check
			 if (rhs_temp.ml_code > -1){
				fprintf(errlogfp, "%d\n", 4); success=0;  // invalid argument
			 }                                           // end error check
		  }
		}
	 }
//	 strset(symbol,' ');
   memset(symbol,0,strlen(symbol));
  }
  fclose(infp); fclose(errlogfp); return(success);
} // end pass1

void p1_err_rpt()
{
  FILE *errlogfp;

  system("clear");
  printf("\n Pass 1 Error Report\n");
  if((errlogfp=fopen("/home/jay/repos/j8cm_local/error.log", "r")) == NULL) {
	 printf("cannot open error log\n");
  }
  else {
	 fscanf(errlogfp, "%d", &errcode);
	 while (!feof(errlogfp)) {
		pass1err(errcode);
		fscanf(errlogfp, "%d", &errcode);
	 }
	 fclose(errlogfp);
  }
}

void view_obj(char *fnm)
{
  FILE *infp;
  char fnm_x[70];
  int ac, i;

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".obj");
  system("clear");
  printf("%s\n\n", fnm_x);
  if((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open .obj file\n");
  }
  else {
	 fscanf(infp, "%d%d", &ac, &i);
	 while (!feof(infp)) {
		 printf("%d %d\n", ac, i);
		 fscanf(infp, "%d%d", &ac, &i);
	 }
	 fclose(infp);
  }
}

void view_ldr(char *fnm)
{
  FILE *infp;
  char fnm_x[70];
  int i;

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".ldr");
  system("clear");
  printf("%s\n\n", fnm_x);
  if((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open file\n");
  }
  else {
	 fscanf(infp, "%d", &i);
	 while (!feof(infp)) {
		printf("%03d\n", i);
		fscanf(infp, "%d", &i);
	 }
	 fclose(infp);
  }
}

void view_sum(char *fnm)
{
  FILE *infp;
  int offset, r, i;
  char fnm_x[70], sumstring[WORDLEN],
		 s1[WORDLEN], s2[WORDLEN], s3[WORDLEN];

  strcpy(fnm_x, fnm);
  strcat(fnm_x, ".sum");
  system("clear");
  printf("%s\n\n", fnm_x);
  if((infp=fopen(fnm_x, "r")) == NULL) {
	 printf("cannot open file\n");
  }
  else {
	 fscanf(infp, "%s", sumstring);
	 while (!feof(infp)) {
		offset = atoi(sumstring);
		fscanf(infp, "%s", sumstring);
		r = atoi(sumstring);
		if (offset == 3) {                      // two rightmost columns only
		  fscanf(infp, "%s", sumstring); strcpy(s2, sumstring);
		  fscanf(infp, "%s", sumstring); strcpy(s3, sumstring);
		  screen_out(offset, r, adrctr, obj_ins, obj_arg, s1, s2, s3);
		}
		else if (offset == 2) {                 // three rightmost
		  fscanf(infp, "%s", sumstring); strcpy(s1, sumstring);
		  fscanf(infp, "%s", sumstring); strcpy(s2, sumstring);
		  fscanf(infp, "%s", sumstring); strcpy(s3, sumstring);
		  screen_out(offset, r, adrctr, obj_ins, obj_arg, s1, s2, s3);
		}
		else if (offset == 1) {                 // all columns
		  fscanf(infp, "%d", &i); adrctr = i;
		  fscanf(infp, "%d", &i); obj_ins = i;
		  fscanf(infp, "%d", &i); obj_arg = i;
		  fscanf(infp, "%s", sumstring);
		  if (strstr(sumstring, ":") != NULL) {  // is label
			 strcpy(s1, sumstring);
			 fscanf(infp, "%s", sumstring); strcpy(s2, sumstring);
			 fscanf(infp, "%s", sumstring); strcpy(s3, sumstring);
		  }
		  else {
//			 strset(s1, ' ');                    // no label
       memset(s1,0,strlen(s1));
			 strcpy(s2, sumstring);
			 fscanf(infp, "%s", sumstring); strcpy(s3, sumstring);
		  }
		 screen_out(offset, r, adrctr, obj_ins, obj_arg, s1, s2, s3);
		}     											 // end if  -all columns
		else if (offset == 4) {                 // no arguments
		  fscanf(infp, "%d", &i); adrctr = i;
		  fscanf(infp, "%d", &i); obj_ins = i;
		  fscanf(infp, "%s", sumstring);
		  if (strstr(sumstring, ":") != NULL) {  // is label
			 strcpy(s1, sumstring);
			 fscanf(infp, "%s", sumstring); strcpy(s2, sumstring);
//			 strset(s3, ' ');
       memset(s3,0,strlen(s3));
		  }
		  else {                                // no label
//			 strset(s1, ' '); strcpy(s2, sumstring); strset(s3, ' ');
       memset(s1,0,strlen(s1)); strcpy(s2, sumstring); memset(s3,0,strlen(s3));
		  }
		  screen_out(offset, r, adrctr, obj_ins, obj_arg, s1, s2, s3);
		}
	 }                                         // end if - no arguments
	 fclose(infp);
  }
  printf("\n\n - any key to continue -\n");
  xchoice = getchar();
}

void post_comp(char *fnm, int vw_files)
{
  while (xchoice != 54) {
	 system("clear");
	 printf("       Post Compile Reports\n");
	 printf(" compiled file - %s.asm\n", fnm);
	 printf("\n 1. Pass 1 error report\n");
	 printf(" 2. View symbol tree\n");
	 if (vw_files) {
		printf(" 3. View object file\n");
		printf(" 4. View loader file\n");
		printf(" 5. View summary file\n");
	 }
	 printf(" 6. Return to main menu\n");
   set_conio_terminal_mode();
  	 xchoice = getchar();
   reset_terminal_mode();
	 if (xchoice == 49) {
		p1_err_rpt(); xchoice = getchar();
	 }
	 if (xchoice == 50) {
		system("clear"); symtreeprint(); xchoice = getchar();
	 }
	 if ((xchoice == 51)&&(vw_files)) {
		view_obj(fnm); xchoice = getchar();
	 }
	 if ((xchoice == 52)&&(vw_files)) {
		view_ldr(fnm); xchoice = getchar();
	 }
	 if ((xchoice == 53)&&(vw_files)) {
		view_sum(fnm); xchoice = getchar();
	 }
  }
}

int compile(char *fnm)
{
  int pass2enable, c_flag;

  system("clear");
  gotoxy(5,4); printf("\nPass 1 Running\n");
  gotoxy(5,6);printf("Source file is: %s", fnm);
  // printf("\nEnter to continue");
  getchar();
  if (remove("/home/jay/repos/j8cm_local/error.log")==-1)
	 printf("\nError deleting old error log");
  symboltreeinit();
  pass2enable = pass1(fnm);
  gotoxy(5,9); printf("Symbol Table Created");
  write_symbol(fnm);
  if (pass2enable == 1) {
	 gotoxy(5,11); printf("\nPass 1 Complete");
	 gotoxy(5,12); printf("\nPass 2 Running");
	 pass2(fnm);
	 gotoxy(5,15); printf("\nPass 2 Complete\n");
  }
  else {
	 gotoxy(5,10); printf("Pass 1 Errors");
  }
  gotoxy(15,18); printf(" - any key to continue -\n");
  xchoice = getchar();
  post_comp(fnm, pass2enable);
  c_flag = 1+pass2enable;
  return(c_flag);
}

void setdir(char *pn)
{
  char p[32];
  system("clear"); printf("Enter device name>");
  scanf("%s", p); strcpy(pn, p);
}

int main(void)
{
  int pc_enable=0;
  char pathname[PATHLEN], psname[20], f[PATHLEN];

  strcpy(pathname, "/home/jay/repos/j8cm_local/");
  inst_treeinit(); read_inst();
  while (xchoice != 121) {
	 system("clear");
	 printf("       J8CM Assembly Language Compiler %1.1f\n", RELNUM);
	 printf(" main menu\n");
	 printf("\n 1. compile file\n");
	 printf(" 2. set device name(%s)\n", pathname);
	 if (pc_enable) printf(" 3. post-compile reports(%s)\n", pathname);
	 printf(" 4. quit\n");
	 xchoice = getchar();
	 if (xchoice == 49) {
		printf("\nEnter name(without extender) of source file - %s",
				 pathname);
		scanf("%s", psname); strcpy(f, pathname); strcat(f, psname);
		pc_enable = compile(f);
	 }
	 if (xchoice == 50) {
		setdir(pathname);
		pc_enable = 0;
	 }
	 if ((xchoice == 51) && (pc_enable)) post_comp(f, pc_enable-1);
	 if (xchoice == 52) {
//		printf(" Are you sure you wish to quit?(y/n)\n");
//		xchoice = getchar();
   exit(0);
	 }
  }
  system("clear"); printf("\n\n  end.");
  return 0;
}
