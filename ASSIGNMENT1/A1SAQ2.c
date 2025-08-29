

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child Process:\n");
        printf("PID: %d, Parent PID: %d\n", getpid(), getppid());

        
        char *args[] = {"ls", "-l", NULL};

        
        execvp("ls", args);

    
        perror("execvp failed");
    } else {
    
        int status;
      

        printf("\nParent Process:\n");
        printf("PID: %d\n", getpid());
        printf("Child terminated, parent resumes execution.\n");
    }

    return 0;
}

