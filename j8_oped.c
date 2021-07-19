// j8_oped.cpp
// Jay Babel
// cmsc265
// Spring 1995
// instr.set file editor for J8CM Compiler j8cm_alc.cpp
// instr.set contains the instruction set translations
// this utility is used to edit that file.
// this program ads num_args to the instruction node.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>

#define WORDLEN 29
#define RELNUM  1.0

int xchoice;

// right hand side node
struct rhs_node
  { int ml_code,
		  num_args; };

struct rhs_node *rhs_work;


// binary search tree inst_node
struct inst_node
  {  char op_code[WORDLEN];
	  struct rhs_node rhs;
	  struct inst_node *l, *r; };

struct inst_node *inst_t, *inst_head, *inst_z;


void inst_treeinit()
  {
//	 rhs_work = (struct rhs_node *) malloc(sizeof *rhs_work);
	 inst_z = (struct inst_node *) malloc(sizeof *inst_z);
	 inst_z->l = inst_z; inst_z->r = inst_z;
	 inst_z->rhs.ml_code = -1;
	 inst_z->rhs.num_args = -1;
	 inst_head = (struct inst_node *) malloc(sizeof *inst_head);
	 inst_head->r = inst_z; strcpy(inst_head->op_code,"");
  }


void inst_treeinsert(char lhs[WORDLEN], int m_code, int n_args)
  {
	 struct inst_node *parent, *x;
	 parent = inst_head; x = inst_head->r;
	 while (x != inst_z)
		{ parent = x; x = (0 > strcmp(lhs, x->op_code)) ? x->l : x->r; }
	 x = (struct inst_node *) malloc(sizeof *x);
	 strncpy(x->op_code, lhs, WORDLEN);
	 x->rhs.ml_code = m_code;
	 x->rhs.num_args = n_args;
	 x->l = inst_z;
	 x->r = inst_z;
	 if (0 > strcmp(lhs, parent->op_code)) parent->l = x; else parent->r = x;
  }


void inst_treedelete(char lhs[WORDLEN])
  {
    struct inst_node *c, *parent, *x;
	 strcpy(inst_z->op_code, lhs);
	 parent = inst_head; x = inst_head->r;
	 while (strcmp(lhs, x->op_code))
      { parent = x; x = (0 > strcmp(lhs, x->op_code)) ? x->l : x->r; }
	 inst_t = x;
	 if (inst_t->r == inst_z) x = x->l;
	 else if (inst_t->r->l == inst_z) { x = x->r; x->l = inst_t->l; }
	 else {
		c = x->r;
		while (c->l->l != inst_z)
		  c = c->l;
		x = c->l; c->l = x->r;
		x->l = inst_t->l; x->r = inst_t->r;
	 }
	 free(inst_t);
	 if (0 > strcmp(lhs, parent->op_code)) parent->l = x; else parent->r = x;
  }


void delete_inst()
{
  char k[WORDLEN], t[WORDLEN];
  system("clear");
  printf("enter op_code to delete ");
  scanf("%s", t);
  strcpy(k, t);
  inst_treedelete(k);
}


// start rule tree print routines

void printinst_node(struct inst_node *x)
  {
	 printf("  %s%11d%12d\n", x->op_code, x->rhs.ml_code, x->rhs.num_args);
  }

void inst_treeprintr(struct inst_node *x)
  {
	 if (x != inst_z)
      {
	inst_treeprintr(x->l);
	printinst_node(x);
	inst_treeprintr(x->r);
      }
  }

int kbhit()
	 {
	     struct timeval tv = { 0L, 0L };
	     fd_set fds;
	     FD_ZERO(&fds);
	     FD_SET(0, &fds);
	     return select(1, &fds, NULL, NULL, &tv);
	 }

void inst_treeprint()
{
  system("clear");
  printf("      Op-code List\n");
  printf("Mnemonic   ML Code    #Arguments\n");
  printf("--------------------------------\n");
  inst_treeprintr(inst_head->r);
  while (!kbhit()) {
  }
}

// end print routines

struct rhs_node inst_treesearch(char lhs[WORDLEN])
{
  struct inst_node *x;
  struct rhs_node r;

  r.ml_code = -1;
  r.num_args = -1;
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


void search_inst()
{
  struct rhs_node i;
  char lhs[WORDLEN], t[WORDLEN];
  system("clear");
  printf("enter op_code value ");
  scanf("%s", t);
  strcpy(lhs, t);
  i = inst_treesearch(lhs);
  if (i.ml_code == -1) printf("op-code not found\n");
	 else printf("\n  %s contains %d %d\n", lhs, i.ml_code, i.num_args);
  while (!kbhit()) {
  }
}

void load_inst()
{
  int mi, na;
  char k[WORDLEN], t[WORDLEN], c[2];
  strcpy(c, "y");
  system("clear");
  do {
	 printf("enter op_code value ");
	 scanf("%s", t);
	 strcpy(k, t);
	 printf("enter ml_code ");
	 scanf("%d", &mi);
	 printf("enter number of arguments ");
	 scanf("%d", &na);
	 inst_treeinsert(k, mi, na);
	 do {
      printf("Add another? (y/n) ");
      scanf("%s", c);
    } while (strcmp(c, "y") && strcmp(c, "n"));
  } while (strcmp(c, "n"));
}


// routines to save and retrieve a:instr.set file
void read_inst()
{
  FILE *infp;
  int x;
  char op[WORDLEN];
  system("clear");
  printf("\n READING\n");
  if((infp=fopen("/home/jay/repos/j8cm_local/xinstr.set", "r")) == NULL) {
	 printf("cannot open file\n");
  }
  while (!feof(infp)) {
	 fscanf(infp, "%s%d%d\n", op, &xchoice, &x);
	 inst_treeinsert(op, xchoice, x);
//	 strset(op,' ');
   memset(op,0,strlen(op));
  }
  fclose(infp);
}


// start rule tree write routines
void wrt_inst_node(FILE *fp, struct inst_node *x)
  {
	 fprintf(fp, "%s %d %d\n", x->op_code, x->rhs.ml_code, x->rhs.num_args);
  }

void inst_tree_wrtr(FILE *fp, struct inst_node *x)
  {
	 if (x != inst_z) {
		inst_tree_wrtr(fp, x->l);
		wrt_inst_node(fp, x);
		inst_tree_wrtr(fp, x->r);
	 }
  }

void write_inst()
{
  FILE *fp;
  system("clear");
  printf("\n WRITING\n");
  if((fp=fopen("/home/jay/repos/j8cm_local/xinstr.set", "wb")) == NULL) {
	 printf("cannot open file\n");
  }
  inst_tree_wrtr(fp, inst_head->r);
  fclose(fp);
}
// end write routines


void iomain()
{
  while (xchoice != 51) {
	 system("clear");
	 printf("    Instruction Set File I/O\n\n");
	 printf(" main menu\n\n");
	 printf(" 1. read instruction set file\n");
	 printf(" 2. write instruction set file\n");
	 printf(" 3. return main menu\n");
	 xchoice = getchar();
	 if (xchoice == 49) read_inst();
	 if (xchoice == 50) write_inst();
  }
  system("clear");
  printf("\n\n  end.");
}

int main(void)
{
  char c[2];

  inst_treeinit();
  while (xchoice != 121) {
	 system("clear");
	 printf("    J8CM AL Compiler Op-code Editor %1.1f\n\n", RELNUM);
    printf(" main menu\n\n");
    printf(" 1. new rule tree\n");
	 printf(" 2. add rules\n");
	 printf(" 3. delete rule\n");
    printf(" 4. view rules\n");
	 printf(" 5. search rule\n");
	 printf(" 6. rule I\\O\n");
    printf(" 9. quit\n");
	 xchoice = getchar();
	 if (xchoice == 49) {
      do {
        printf("Are you sure? (y/n) ");
        scanf("%s", c);
      } while (strcmp(c, "y") && strcmp(c, "n"));
		if (!strcmp(c, "y")) inst_treeinit();
    }
	 if (xchoice == 50) load_inst();
	 if (xchoice == 51) delete_inst();
	 if (xchoice == 52) inst_treeprint();
	 if (xchoice == 53) search_inst();
	 if (xchoice == 54) iomain();
	 if (xchoice == 57) {
	//	printf(" Are you sure you wish to quit?(y/n)\n");
	//	xchoice = getchar();
     exit(0);
	 }
  }
  system("clear");
  printf("\n\n  end.");
  return 0;
}
