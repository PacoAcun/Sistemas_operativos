ENTRY(_start)

MEMORY
{
    ram   : ORIGIN = 0x82000000, LENGTH = 64K
    stack : ORIGIN = 0x82010000, LENGTH = 8K
}

# Se definen las partes del programa
SECTIONS
{
    . = ORIGIN(ram);

# Se define la tabla de vectores que hace el programa en una interrupcion
    .vectors : {
        *(.vectors)
    } > ram

# Se define el codigo del programa
    .text : {
        *(.text*)
    } > ram

# Se definen las variables solo de lectura como las constantes
    .rodata : {
        *(.rodata*)
    } > ram

# Se define las variables inicializadas
    .data : {
        *(.data*)
    } > ram

# Se definen las variables no inicializadas, se les asigna 0 como valor
    .bss : {
        _bss_start_ = .;
        *(.bss*)
        *(COMMON)
        _bss_end_ = .;
    } > ram

# Se define el limite del stack
    .stack : {
        _stack_top = .;
        . = . + 0x2000;
        _stack_bottom = .;
    } > stack
}
