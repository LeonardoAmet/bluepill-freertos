ENTRY(reset_handler)

MEMORY
{
  rom (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
  .text : {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
  } >rom

  .data : {
    *(.data*)
  } >ram AT >rom

  .bss : {
    *(.bss*)
  } >ram
}
