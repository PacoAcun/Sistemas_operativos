#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>

void create_process() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child Process: PID=%d, Parent PID=%d\n", getpid(), getppid());
        exit(0);
    } else {
        printf("Parent Process: PID=%d\n", getpid());
        wait(NULL);
    }
}

void process_sync() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child Process: PID=%d, Parent PID=%d\n", getpid(), getppid());
        sleep(2);
        printf("Child exiting...\n");
        exit(0);
    } else {
        wait(NULL);
        printf("Parent Process: Child has finished execution.\n");
    }
}

void process_pipe() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[1]);
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        printf("Child Process: Received \"%s\"\n", buffer);
        close(fd[0]);
        exit(0);
    } else {
        close(fd[0]);
        char message[] = "Hello from Parent";
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        wait(NULL);
    }
}

void multiple_processes() {
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child %d: PID=%d, Parent PID=%d\n", i + 1, getpid(), getppid());
            exit(0);
        }
    }
    while (wait(NULL) > 0);
}

void shared_memory() {
    int shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0666);
    char *shared_memory = (char *)shmat(shmid, NULL, 0);
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        printf("Child Process: Read \"%s\"\n", shared_memory);
        shmdt(shared_memory);
        exit(0);
    } else {
        strcpy(shared_memory, "Shared Memory Example");
        wait(NULL);
        shmdt(shared_memory);
        shmctl(shmid, IPC_RMID, NULL);
    }
}

int main() {
    printf("\n1. Creating a New Process\n");
    create_process();
    
    printf("\n2. Synchronizing Parent and Child\n");
    process_sync();
    
    printf("\n3. Inter-Process Communication Using Pipes\n");
    process_pipe();
    
    printf("\n4. Creating Multiple Child Processes\n");
    multiple_processes();
    
    printf("\n5. Shared Memory Example\n");
    shared_memory();
    
    return 0;
}