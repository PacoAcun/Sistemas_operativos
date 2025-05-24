ENTRY(_start)

MEMORY
{
    ram   : ORIGIN = 0x82200000, LENGTH = 64K
    stack : ORIGIN = 0x82220000, LENGTH = 8K
}

SECTIONS
{
    . = ORIGIN(ram);

    .text : {
        *(.text*)
        *(.rodata*)
    } > ram

    .data : {
        *(.data*)
    } > ram

    .bss : {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > ram

    .stack : {
        __stack_bottom = .;
        . = . + 0x2000;
        __stack_top = .;
    } > stack
}

