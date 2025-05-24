MEMORY
{
    ram : ORIGIN = 0x82100000, LENGTH = 64K
    stack : ORIGIN = 0x82110000, LENGTH = 8K
}

SECTIONS
{
    . = ORIGIN(ram);

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > ram

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > ram

    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > ram

    .stack :
    {
        __stack_bottom = .;
        . = . + 0x2000; /* 8KB stack */
        __stack_top = .;
    } > stack
}

