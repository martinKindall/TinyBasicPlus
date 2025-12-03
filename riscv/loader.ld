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
      *(.text .text*)
   } >IMEM

   .rodata :
   {
      . = ALIGN(4);
      *(.rodata .rodata*)
   } >DMEM

   .data :
   {
      . = ALIGN(4);
      *(.data .data*)
   } >DMEM

   /* * SMALL DATA: The GP points here.
    * We offset GP by 0x800 (2KB) to maximize the +/- 2KB range.
    */
   .sdata :
   {
      . = ALIGN(4);
      PROVIDE( __global_pointer$ = . + 0x800 );
      *(.sdata .sdata*)
      *(.srodata .srodata*)
   } >DMEM

   .sbss :
   {
      . = ALIGN(4);
      *(.sbss .sbss*)
      *(.scommon)
   } >DMEM

   .bss :
   {
      . = ALIGN(4);
      *(.bss .bss*)
      *(COMMON)
   } >DMEM
}
