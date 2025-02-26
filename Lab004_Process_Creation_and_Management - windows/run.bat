@echo off
setlocal

echo Cleaning up previous build files...
del /Q *.exe *.o 2>nul

echo Compiling task 1: Process Creation...
gcc -o parent.exe parent.c
gcc -o child.exe child.c

echo Compiling task 2: Process Synchronization...
gcc -o parent_sync.exe parent_sync.c
gcc -o child_sync.exe child_sync.c

echo Compiling task 3: Pipe Communication...
gcc -o parent_pipe.exe parent_pipe.c
gcc -o child_pipe.exe child_pipe.c

echo Compiling task 4: Multiple Children...
gcc -o parent_multi.exe parent_multi.c
gcc -o child_multi.exe child_multi.c

echo Compiling task 5: Shared Memory...
gcc -o parent_shmem.exe parent_shmem.c
gcc -o child_shmem.exe child_shmem.c

echo.
echo Running Task 1: Process Creation...
parent.exe
echo.

echo Running Task 2: Process Synchronization...
parent_sync.exe
echo.

echo Running Task 3: Pipe Communication...
parent_pipe.exe
echo.

echo Running Task 4: Multiple Children...
parent_multi.exe
echo.

echo Running Task 5: Shared Memory...
parent_shmem.exe
echo.

echo All tasks completed.
