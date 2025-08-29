
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child Process ID: %d\n", getpid());
        printf("Hello World\n");
    } else {
        // Parent process
        printf("Parent Process ID: %d\n", getpid());
        printf("Hi\n");
    }

    return 0;
}

