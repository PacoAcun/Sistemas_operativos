#!/bin/bash

# Salir inmediatamente si un comando falla
set -e

# Limpiar archivos de compilación anteriores
echo "Limpiando archivos de compilación anteriores..."
rm -f root.o main.o os.o p2.elf p2.bin stdio.o string.o

# Ubicación de los archivos compartidos
SHARED_PATH="../lib"

# Ensamblar el archivo de inicio desde p1
echo "Ensamblando root.s..."
arm-none-eabi-as -mcpu=cortex-a8 -o root.o root.s

# Compilar main.c desde p2 (local)
echo "Compilando main.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -marm -o main.o main.c

# Compilar os.c
echo "Compilando os.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -marm -o os.o ${SHARED_PATH}/os.c -I${SHARED_PATH}

# Compilar stdio.c desde lib
echo "Compilando stdio.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -marm -o stdio.o ${SHARED_PATH}/stdio.c -I${SHARED_PATH}

# Compilar string.c desde lib
echo "Compilando string.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -marm -o string.o ${SHARED_PATH}/string.c -I${SHARED_PATH}

# Enlazar los archivos objeto usando el linker.ld desde p1
echo "Enlazando archivos objeto..."
arm-none-eabi-ld -T linker.ld -o p2.elf root.o main.o os.o stdio.o string.o

# Convertir el archivo ELF a binario
echo "Convirtiendo ELF a binario..."
arm-none-eabi-objcopy -O binary p2.elf p2.bin

echo "Compilación completada exitosamente."


