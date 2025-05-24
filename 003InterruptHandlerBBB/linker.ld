ENTRY(_start)

SECTIONS
{
    . = 0x00010000; /* Starting address in RAM */

    .text :
    {
        *(.text*)  /* Incluye todas las secciones .text de los archivos objeto */
    }

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        *(.data*)  /* Incluye todas las secciones .data de los archivos objeto */
    }

    .bss :
    {
        __bss_start = .;
        *(.bss*)   /* Incluye todas las secciones .bss de los archivos objeto */
        __bss_end = .;
    }
}
