ENTRY(_start)

SECTIONS
{
    /* Dirección de inicio de la memoria */
    . = 0x00010000;

    /* Sección de código */
    .text :
    {
        *(.text*)        /* Incluye todas las secciones de código */
        *(.rodata*)      /* Incluye datos de solo lectura */
    }

    /* Sección de datos inicializados */
    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        _data_start = .; /* Marca el inicio de la sección .data */
        *(.data*)        /* Incluye todas las secciones de datos inicializados */
        _data_end = .;   /* Marca el final de la sección .data */
    }

    /* Sección de datos no inicializados (BSS) */
    .bss :
    {
        _bss_start = .;  /* Marca el inicio de la sección .bss */
        *(.bss*)         /* Incluye todas las secciones de datos no inicializados */
        *(COMMON)        /* Incluye símbolos comunes */
        _bss_end = .;    /* Marca el final de la sección .bss */
    }

    /* Opcional: definir un área de pila */
    .stack :
    {
        . = . + 0x1000;  /* Reserva espacio para la pila (4 KB) */
    }
}