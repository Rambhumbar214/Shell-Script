
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        
        printf("Child process executing 'ls' command:\n");
        execl("/bin/ls", "ls", "-l", NULL);
        perror("execl failed");
    } else {
        
        sleep(5); 
        printf("Parent process (PID %d) wakes up after child.\n", getpid());
    }

    return 0;
}

