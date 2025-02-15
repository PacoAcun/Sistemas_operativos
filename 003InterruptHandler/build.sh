#!/bin/bash

set -e

echo "Cleaning up previous build files..."
rm -f root.o main.o os.o lab003.elf lab003.bin

echo "Assembling startup.s..."
arm-none-eabi-gcc -c root.s -o root.o

echo "Compilando uart.c..."
arm-none-eabi-gcc -c uart.c -o uart.o

echo "Compiling main.c..."
arm-none-eabi-gcc -c main.c -o main.o

echo "Compilando os.c..."
arm-none-eabi-gcc -c os.c -o os.o

echo "Linking object files..."
arm-none-eabi-ld -T linker.id -o lab003.elf root.o main.o os.o uart.o

echo "Converting ELF to binary..."
arm-none-eabi-objcopy -O binary lab003.elf lab003.bin

echo "Running QEMU..."
qemu-system-arm \
-M versatilepb \
-m 128M \
-nographic \
-kernel lab003.bin


