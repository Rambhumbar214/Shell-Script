#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    pid_t pid;
    int ret;

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child process started. PID = %d\n", getpid());

        int old_nice = nice(0);  // get current nice value
        printf("Child current nice value: %d\n", old_nice);

        // Try to reduce nice value (i.e., increase priority)
        ret = nice(-5); // requires root privilege
        if (ret == -1 && errno != 0) {
            perror("Child nice(-5) failed");
        } else {
            printf("Child new nice value: %d\n", ret);
        }

        // Simulate some work
        for (int i = 0; i < 5; i++) {
            printf("Child is working (iteration %d)\n", i+1);
            sleep(1);
        }

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent process. PID = %d, waiting for child...\n", getpid());
        wait(NULL);
        printf("Child has finished execution.\n");
    }

    return 0;
}

