// j8cm_cpu.cpp
// Central Processor Code
// Jay's 8-bit Computer Model
// Jay Babel
// contains:
//     code for fetch and execute cycles
//     instruction set code
//     decode routines (convert 8-bit binary to decimal)
// relnum 2.0  - PSW, XREG, YREG are added to CPU

						  // decode routines
int pc_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+PC[i]*pow(2,(i-1));
  return a;
}

int mar_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
    a=a+MAR[i]*pow(2,(i-1));
  return a;
}

struct i_node ir_decode()
{
  struct i_node inst;
  int i;
  inst.opcode=0;
  inst.adr_mode=0;
  for (i=1; i<6; ++i)
	 inst.opcode=inst.opcode+IR[i]*pow(2,(i-1));
  for (i=1; i<3; ++i)
	 inst.adr_mode=inst.adr_mode+IR[5+i]*pow(2,(i-1));
  inst.reg = IR[8];
  return (inst);
}

int sp_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+SP[i]*pow(2,(i-1));
  return a;
}

int acc_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+accumulator[i]*pow(2,(i-1));
  return a;
}

int xreg_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+xreg[i]*pow(2,(i-1));
  return a;
}

int yreg_decode()
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+yreg[i]*pow(2,(i-1));
  return a;
}

int mem_decode(int l)
{
  int a, i;
  a=0;
  for (i=1; i<9; ++i)
	 a=a+memory[l] [i]*pow(2,(i-1));
  return a;
}

void encode_acc(int d)
{
  int i;
  struct word_node to_acc;

  to_acc = twos_comp(d);
  for (i=1; i<9; ++i)
	 accumulator[i] = to_acc.temp_word[i];
}
						  // end decode/encode routines


void increment_PC()
{
  int i, d;
  d=pc_decode();
  d++;
  for (i=1; i<9; ++i){
    PC[i] = d%2;
	 d=d/2;
  }
}

void decrement_SP()
{
  int i, j;
  i=sp_decode();                   // decimal value of SP
  i--;                             // decr SP
  if (i < 0) i = 255;              // new stack
  for (j=1; j<9; ++j){             // convert dec to bin
	 SP[j] = i%2;                   //   and reload SP
	 i=i/2;
  }
}

void increment_SP()
{
  int i, j;
  i=sp_decode();                   // decimal value of SP
  i++;                             // incr SP
  if (i > 255) i = 0;              // empty stack
  for (j=1; j<9; ++j){             // convert dec to bin
	 SP[j] = i%2;                   //   and reload SP
	 i=i/2;
  }
}

//=========================== Address Modes ===================================
int get_address(int a_mode, int gp_reg)
{
  int a, i, j;
  i=pc_decode();
  if (a_mode < 2) {  // not register mode
	 increment_PC();  // PC is incremented
	 for (j=1; j<9; ++j)
		MAR[j] = memory[i] [j];
	 i=mar_decode();
  }
  if (a_mode == 1) {   // memory indirect
	 for (j=1; j<9; ++j)
		MAR[j] = memory[i] [j];
	 i=mar_decode();
  }
  if (a_mode == 3) {   // register indirect
	 for (j=1; j<9; ++j) {
		if (gp_reg == 1) MAR[j] = yreg [j];
		  else MAR[j] = xreg [j];
	 }
	 i=mar_decode();
  }
  a=i;
  return (a);
}

// *************************  instruction set  **********************
// Data Transfer Instructions

void load_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg) accumulator[j] = yreg [j];
		  else accumulator[j] = xreg [j];
  }
  else
	 for (j=1; j<9; ++j)
		accumulator[j] = memory[i] [j];
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
}

void load_xreg(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (!gp_reg) xreg[j] = xreg [j];
		if (gp_reg == 1) xreg[j] = yreg [j];
  }
  else
	 for (j=1; j<9; ++j)
		xreg[j] = memory[i] [j];
  FLG.n_flag = (xreg[8] == 1);
  FLG.z_flag = (xreg_decode() == 0);
}

void load_yreg(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (!gp_reg) yreg[j] = xreg [j];
		if (gp_reg == 1) yreg[j] = yreg [j];
  }
  else
	 for (j=1; j<9; ++j)
      yreg[j] = memory[i] [j];
  FLG.n_flag = (yreg[8] == 1);
  FLG.z_flag = (yreg_decode() == 0);
}


void store_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg == 1) yreg [j] = accumulator[j];
		  else xreg [j] = accumulator[j];
  }
  else
	 for (j=1; j<9; ++j)
		memory[i] [j] = accumulator[j];
}

void store_xreg(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg == 1) yreg [j] = xreg[j];
		  else xreg [j] = xreg[j];
  }
  else
	 for (j=1; j<9; ++j)
		memory[i] [j] = xreg[j];
}

void store_yreg(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg == 1) yreg [j] = yreg[j];
		  else xreg [j] = yreg[j];
  }
  else
	 for (j=1; j<9; ++j)
		memory[i] [j] = yreg[j];
}

//          input and output must be through memory
//             register direct is not supported
void input_io(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  for (j=1; j<9; ++j)
	 memory[i] [j] = io_in[in_mrkr] [j];
  in_mrkr++;
  if (in_mrkr > iolen) in_mrkr=0;
}

void output_io(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  for (j=1; j<9; ++j)
	 io_out[out_mrkr] [j] = memory[i] [j];
  out_mrkr++;
  if (out_mrkr > iolen) out_mrkr=0;
}

void push_stack()
{
  int i, j;
  decrement_SP();
  i=sp_decode();                       // decimal value of SP
  for (j=1; j<9; ++j)                  // copy accumulator to
	  memory[i] [j] = accumulator[j];   //   stack memory location
}

void pop_stack()
{
  int i, j;
  i=sp_decode();                       // decimal value of SP
  for (j=1; j<9; ++j) {                 // copy stack memory location
	  accumulator[j] = memory[i] [j];   //   to accumulator
    memory[i] [j] = 0;
  }
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
  increment_SP();
}

// End Data Transfer Instructions

// Arithmetic Instructions

void inc_acc()
{
  int d;
  d=acc_decode();
  d++;
  FLG.z_flag = (d == 0);
  if (d > 127) FLG.c_flag = 1;
	 else FLG.c_flag = 0;
  encode_acc(d);
  FLG.n_flag = (accumulator[8] == 1);
}

void dec_acc()
{
  int d;
  d=acc_decode();
  d--;
  FLG.z_flag = (d == 0);
  if (d < -128) FLG.c_flag = 0;
	 else FLG.c_flag = 1;
  encode_acc(d);
  FLG.n_flag = (accumulator[8] == 1);
}

void add_to_acc(int a_mode, int gp_reg)
{
  int m, a, i;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (m=1; m<9; ++m)
		if (gp_reg == 1) MAR[m] = yreg [m];
		  else MAR[m] = xreg [m];
  }
  else
	 for (m=1; m<9; ++m)
		MAR[m] = memory[i] [m];
  i=mar_decode();
  m=mem_decode(i);
  a=acc_decode();
  a=a+m;
  FLG.z_flag = (a == 0);
  if (a > 127) FLG.c_flag = 1;
	 else FLG.c_flag = 0;
  encode_acc(a);
  FLG.n_flag = (accumulator[8] == 1);
}

void sub_from_acc(int a_mode, int gp_reg)
{
  int m, a, i;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (m=1; m<9; ++m)
		if (gp_reg == 1) MAR[m] = yreg [m];
		  else MAR[m] = xreg [m];
  }
  else
	 for (m=1; m<9; ++m)
		MAR[m] = memory[i] [m];
  i=mar_decode();
  m=mem_decode(i);
  a=acc_decode();
  a=a-m;
  FLG.z_flag = (a == 0);
  if (a < -128) FLG.c_flag = 0;
	 else FLG.c_flag = 1;
  encode_acc(a);
  FLG.n_flag = (accumulator[8] == 1);
}

void arth_shift_r()  // arithmetic shift
{
  int i, signbit;
  signbit = accumulator[8];
  for (i=1; i<8; ++i) {
	 accumulator[i] = accumulator[i+1];
  }
  accumulator[8] = signbit;
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
}

void arth_shift_l()
{
  int i;
  for (i=8; i>1; --i) {
	 accumulator[i] = accumulator[i-1];
  }
  accumulator[1] = 0;
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
}
// End Arithmetic Instructions

// Logic Instructions

void logical_shift_r()
{
  int i;
  for (i=1; i<8; ++i) {
	 accumulator[i] = accumulator[i+1];
  }
  accumulator[8] = 0;
  FLG.n_flag = 0;
  FLG.z_flag = (acc_decode() == 0);
}

void rotate_r()
{
  int i, t;
  t = FLG.c_flag;
  FLG.c_flag = accumulator[1];
  for (i=1; i<8; ++i) {
	 accumulator[i] = accumulator[i+1];
  }
  accumulator[8] = t;
  FLG.n_flag = accumulator[8];
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = ((FLG.n_flag || FLG.c_flag) && !(FLG.n_flag && FLG.c_flag));
}

void rotate_l()
{
  int i, t;
  t = FLG.c_flag;
  FLG.c_flag = accumulator[8];
  for (i=8; i>1; --i) {
	 accumulator[i] = accumulator[i-1];
  }
  accumulator[1] = t;
  FLG.n_flag = accumulator[8];
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = ((FLG.n_flag || FLG.c_flag) && !(FLG.n_flag && FLG.c_flag));
}

void and_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg) accumulator[j] = (yreg [j] && accumulator[j]);
		  else accumulator[j] = (xreg [j] && accumulator[j]);
  }
  else
	 for (j=1; j<9; ++j)
		accumulator[j] = (memory[i] [j] && accumulator[j]);
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = 0;
}

void or_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg) accumulator[j] = (yreg [j] || accumulator[j]);
		  else accumulator[j] = (xreg [j] || accumulator[j]);
  }
  else
	 for (j=1; j<9; ++j)
		accumulator[j] = (memory[i] [j] || accumulator[j]);
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = 0;
}

void xor_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg) accumulator[j] = ((yreg [j] || accumulator[j]) &&
												!(yreg [j] && accumulator[j]));
		  else accumulator[j] = ((xreg [j] || accumulator[j]) &&
										 !(xreg [j] && accumulator[j]));
  }
  else
	 for (j=1; j<9; ++j)
		accumulator[j] = ((memory[i] [j] || accumulator[j]) &&
								!(xreg [j] && accumulator[j]));
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = 0;
}

void not_acc(int a_mode, int gp_reg)
{
  int i, j;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (j=1; j<9; ++j)
		if (gp_reg) accumulator[j] = !(yreg [j]);
		  else accumulator[j] = !(xreg [j]);
  }
  else
	 for (j=1; j<9; ++j)
		accumulator[j] = !(memory[i] [j]);
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
  FLG.v_flag = 0;
}

// End Logic Instructions


// Conversion Instructions

void clr_acc()
{
  int j;
  for (j=1; j<9; ++j) accumulator[j] = 0;
  FLG.n_flag = 0; FLG.z_flag = 1;
  FLG.v_flag = 0; FLG.c_flag = 0;
}

void neg_acc()
{
  int i;

  i=acc_decode(); i = -i; encode_acc(i);
  FLG.n_flag = (accumulator[8] == 1);
  FLG.z_flag = (acc_decode() == 0);
}

// End Conversion Instructions

// Control Instructions

void jump_immediate()
{
  int i, j;
  i=pc_decode();
  for (j=1; j<9; ++j)
	 PC[j] = memory[i] [j];
}

void gosub()
{
  int i, j, c;
  i=sp_decode();
  c=pc_decode();
  --i;
  if (i < 0) i = 255;
  increment_PC();       // increment PC to point to next inst.
  for (j=1; j<9; ++j)
	 memory[i] [j] = PC[j];
  for (j=1; j<9; ++j)
	 PC[j] = memory [c] [j];
  decrement_SP();
}

void return_gosub()
{
  int i, j;
  i=sp_decode();
  for (j=1; j<9; ++j){
	  PC[j] = memory[i] [j];
          memory[i] [j] = 0;
  }
  increment_SP();
}

void halt()     // same as jump instruction (so far)
{
  int i, j;
  i=pc_decode();
  for (j=1; j<9; ++j)
	 PC[j] = memory[i] [j];
}

// End Control Instructions

// Conditional Instructions

void cmp_to_acc(int a_mode, int gp_reg)
{
  int m, a, i;
  i = get_address(a_mode, gp_reg);
  if (a_mode == 2) {
	 for (m=1; m<9; ++m)
		if (gp_reg == 1) MAR[m] = yreg [m];
		  else MAR[m] = xreg [m];
  }
  else
	 for (m=1; m<9; ++m)
		MAR[m] = memory[i] [m];
  i=mar_decode();
  m=mem_decode(i);
  a=acc_decode();
  FLG.z_flag = (a == m);    // if arg = accumulator set zero flag
  FLG.n_flag = (a < m);     // if arg > accumulator set n_flag
}

void test_n_flag()
{
  int i, j;
  if (FLG.n_flag ==  1) {                         // jump if n_flag = 1
	 i = pc_decode();
	 for (j=1; j<9; ++j) PC[j] = memory[i] [j];
  }
  else increment_PC();
}

void test_z_flag()
{
  int i, j;
  if (FLG.z_flag == 1) {                          // jump if z_flag = 1
	 i = pc_decode();
	 for (j=1; j<9; ++j) PC[j] = memory[i] [j];
  }
  else increment_PC();
}
// End Conditional Instructions

																  // end instruction set

void fetch()
{
  int i, j;
  i_flag=0;
  i_flag_out(56,20);;
	 for (j=1; j<9; ++j) MAR[j] = PC[j];
  i=mar_decode();
  for (j=1; j<9; ++j)
	 IR[j] = memory[i] [j];
  FLG.c_flag = 0;
  PSW[8] = FLG.c_flag;
  increment_PC();
  disp_regs();
}

void execute()
{
  struct i_node instr;
  i_flag=1; i_flag_out(56,20);
  instr = ir_decode();
  // Data Transfer  tot = 10
  if (instr.opcode == 0) input_io(instr.adr_mode, instr.reg);
  if (instr.opcode == 1) output_io(instr.adr_mode, instr.reg);
  if (instr.opcode == 4) load_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 19) load_xreg(instr.adr_mode, instr.reg);
  if (instr.opcode == 20) load_yreg(instr.adr_mode, instr.reg);
  if (instr.opcode == 5) store_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 21) store_xreg(instr.adr_mode, instr.reg);
  if (instr.opcode == 22) store_yreg(instr.adr_mode, instr.reg);
  if (instr.opcode == 17) push_stack();
  if (instr.opcode == 18) pop_stack();

  // Arithmetic/Logic    tot = 13
  if (instr.opcode == 2) add_to_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 3) sub_from_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 10) inc_acc();
  if (instr.opcode == 11) dec_acc();
  if (instr.opcode == 14) logical_shift_r();
  if (instr.opcode == 15) arth_shift_l();
  if (instr.opcode == 16) arth_shift_r();
  if (instr.opcode == 23) rotate_l();
  if (instr.opcode == 24) rotate_r();
  if (instr.opcode == 25) and_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 26) or_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 27) xor_acc(instr.adr_mode, instr.reg);
  if (instr.opcode == 28) not_acc(instr.adr_mode, instr.reg);

  // Control       tot = 4
  if (instr.opcode == 6) jump_immediate();
  if (instr.opcode == 12) gosub();
  if (instr.opcode == 13) return_gosub();
  if (instr.opcode == 9) halt();

  // Conversion     tot = 2
  if (instr.opcode == 29) clr_acc();
  if (instr.opcode == 31) neg_acc();

  // Conditional Instructions   tot = 3
  if (instr.opcode == 7) test_n_flag();
  if (instr.opcode == 8) test_z_flag();
  if (instr.opcode == 30) cmp_to_acc(instr.adr_mode, instr.reg);
  FLG.c_flag = 1;
  PSW[8] = FLG.c_flag; PSW[7] = FLG.v_flag;
  PSW[6] = FLG.n_flag; PSW[5] = FLG.z_flag;
  disp_regs();
}

void step()
{
	 fetch();
	 i_flag_out(56,20);;
	 execute();
	 i_flag_out(56,20);;
}

void run()
{
  struct i_node instr;
  instr.opcode=0;
  while (instr.opcode != 9) {
	 fetch();
	 i_flag_out(56,20);;
	 execute();
	 i_flag_out(56,20);;
	 instr = ir_decode();
  }
}
// end j8cm_cpu.cpp
