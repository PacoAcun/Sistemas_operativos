#!/bin/bash

# Salir inmediatamente si un comando falla
set -e

# Limpiar archivos de compilación anteriores
echo "Limpiando archivos de compilación anteriores..."
rm -f root.o main.o stdio.o string.o os.o calculadora.elf calculadora.bin

# Ensamblar el archivo de inicio
echo "Ensamblando root.s..."
arm-none-eabi-as -o root.o root.s

# Compilar los archivos fuente
echo "Compilando main.c..."
arm-none-eabi-gcc -c -o main.o main.c

echo "Compilando stdio.c..."
arm-none-eabi-gcc -c -o stdio.o stdio.c

echo "Compilando string.c..."
arm-none-eabi-gcc -c -o string.o string.c

echo "Compilando os.c..."
arm-none-eabi-gcc -c -o os.o os.c

# Enlazar los archivos objeto
echo "Enlazando archivos objeto..."
arm-none-eabi-ld -T linker.ld -o calculadora.elf root.o main.o stdio.o string.o os.o

# Convertir el archivo ELF a binario
echo "Convirtiendo ELF a binario..."
arm-none-eabi-objcopy -O binary calculadora.elf calculadora.bin

# Ejecutar en QEMU
echo "Ejecutando en QEMU..."
qemu-system-arm \
-M versatilepb \
-m 128M \
-nographic \
-echr 0x14 \
-kernel calculadora.bin


