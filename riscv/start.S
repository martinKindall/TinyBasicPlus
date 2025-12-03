.section .text
.globl start

start:
  /* * Initialize the Global Pointer (gp)
   * 1. Push current options
   * 2. Disable linker relaxation (critical for gp init)
   * 3. Load the address of the symbol defined in the .ld file
   * 4. Pop options
   */
.option push
.option norelax
  la gp, __global_pointer$
.option pop

  /* Initialize Stack Pointer to the end of DMEM */
  li  sp, 0x5000 

  call main

finish:
	j finish
    
  ebreak
