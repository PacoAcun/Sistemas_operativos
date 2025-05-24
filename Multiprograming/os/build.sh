#!/bin/bash

set -e

echo "Limpiando..."
rm -f os.elf os.bin *.o *.map

echo "Corrigiendo permisos..."
chmod +x *.sh

echo "Compilando root.s..."
arm-none-eabi-as -mcpu=cortex-a8 -o root.o root.s

echo "Compilando os.c (con main)..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -ffreestanding -I../lib -o os.o os.c

echo "Compilando librerías..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -ffreestanding -I../lib -o os_lib.o ../lib/os.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -ffreestanding -I../lib -o stdio.o ../lib/stdio.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -ffreestanding -I../lib -o string.o ../lib/string.c

echo "Enlazando..."
arm-none-eabi-ld -T linker.ld -o os.elf root.o os.o os_lib.o stdio.o string.o

echo "Generando binario..."
arm-none-eabi-objcopy -O binary os.elf os.bin

echo "Tamaños:"
arm-none-eabi-size os.elf

echo "¡Build completo!"




