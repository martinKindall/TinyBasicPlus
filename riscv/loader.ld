MEMORY
{
   IMEM (RX)  : ORIGIN = 0x00000000, LENGTH = 0x4000   /* 16 KB */
   DMEM (RW)  : ORIGIN = 0x00004000, LENGTH = 0x1000   /* 4 KB */
}

SECTIONS
{
   .text :
   {
      . = ALIGN(4);
      start.o (.text)
      *(.text*)
   } >IMEM

   .data :
   {
      *(.data*)
      *(.rodata*)
      *(.bss*)
   } >DMEM
}
