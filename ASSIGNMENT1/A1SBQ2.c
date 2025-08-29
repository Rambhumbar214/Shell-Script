

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        int priority = nice(-5); 
        if (priority == -1) {
            perror("nice failed (need root for negative priority)");
        }

        printf("Child Process (PID: %d), New Priority (Niceness): %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
    } else {
        // Parent process
        printf("Parent Process (PID: %d), Default Priority: %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
    }

    return 0;
}

