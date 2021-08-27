# j8cm
# Jay's 8-bit Computer Model
# v2.0 (the gcc port)

Originally written on Windows 3.1 or 95 (I don't remember) Borland C 4.0
This project is to port the J8CM to Linux gcc.  
It appears to be fully functional in Linux now but I haven't tested everything - there are 32 instructions, 4 addressing modes, and the assembly language compiler.

Demos on YouTube at https://www.youtube.com/channel/UCIGNs_V7qVKOYLUIHZwZUsg/videos

Compile the computer using:  gcc -o j8cm j8cm.c -lm

Compile the assembly language compiler using:  gcc -o j8cm_alc j8cm_alc.c -lm

There are three executable programs that make up the j8cm system:

An executable version of Jay's 8-bit Computer Model. (I usually name it j8cm)  In Windows it was j8cm.exe

An executable for the J8CM Assembly Language Compiler.  In Windows it was named j8cm_alc.exe.

A utility for creating and editing instruction set files used by the J8CM AL Compiler.  It was called j8_oped.exe in Windows.

Source code files:

  j8cm.c - presents main menu to the user. Includes are:
  
      j8cm_clr.c - Data Destruct Menu to clear memory, registers, I/O, etc.
      
      j8cm_pnl.c - j8cm front panel. Routines to display accumulator, program counter, instruction register, etc. Includes:
      
          j8cm_cpu.c - central processing unit code
          
      j8cm_mem.c - code to emulate/display j8cm main memory
      
      j8cm_io.c - I/O panel routines
      
      j8cm_ldr.c - loads compiler output files into main memory directly or via "punched tape reader"
    
  j8cm_alc.c - j8cm assembly language compiler
   
  j8_oped.c - op code editer
  
  j8cm_viewer - program to read and display snapshot file of the J8CM memory.  Enables a view of memory without requiring exot from the front panel (or any other j8cm screen). 
  
