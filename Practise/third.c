#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child: My PID is %d, my parent's PID is %d\n", getpid(), getppid());
        sleep(5); // Sleep to allow parent to exit
        printf("Child: After sleep, my new parent PID is %d (should be 1 if orphaned)\n", getppid());
    } else {
        // Parent process
        printf("Parent: My PID is %d. I'm going to exit now.\n", getpid());
        exit(0); // Exit immediately, making the child an orphan
    }

    return 0;
}

