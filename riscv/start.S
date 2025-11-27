
.section .text

.globl start

start:
	li   sp, 0x5000   # allocate space for stack
	call main

finish:
    j finish
    
	ebreak
