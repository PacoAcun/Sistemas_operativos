#!/bin/bash

# Salir inmediatamente si un comando falla
set -e

# Limpiar archivos de compilaciones anteriores
echo "Limpiando archivos de compilaciones anteriores..."
rm -f root.o main.o os.o uart.o lab003.elf lab003.bin

# Compilar root.s
echo "Compilando root.s..."
arm-none-eabi-as -mcpu=cortex-a8 root.s -o root.o

# Compilar main.c
echo "Compilando main.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -nostartfiles main.c -o main.o

# Compilar os.c
echo "Compilando os.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -nostartfiles os.c -o os.o

# Enlazar archivos objeto
echo "Enlazando archivos objeto..."
arm-none-eabi-ld -T linker.id -o lab003.elf root.o main.o os.o

# Convertir ELF a binario
echo "Convirtiendo ELF a binario..."
arm-none-eabi-objcopy -O binary lab003.elf lab003.bin

echo "Running QEMU..."
qemu-system-arm \
-M versatilepb \
-m 128M \
-nographic \
-echr 0x14 \
-kernel lab003.bin