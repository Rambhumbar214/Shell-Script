#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Create child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child Process:\n");
        printf("PID: %d, Parent PID: %d\n", getpid(), getppid());
        printf("Hello World\n");
    } else {
        // Parent process
        printf("Parent Process:\n");
        printf("PID: %d, Child PID: %d\n", getpid(), pid);
        printf("Hi\n");
    }

    return 0;
}

