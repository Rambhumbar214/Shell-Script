

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD 1024
#define MAX_ARGS 10

void count_command(char mode, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File open failed");
        return;
    }

    int ch, characters = 0, words = 0, lines = 0;
    int in_word = 0;

    while ((ch = fgetc(fp)) != EOF) {
        characters++;
        if (ch == '\n') lines++;
        if (ch == ' ' || ch == '\n' || ch == '\t')
            in_word = 0;
        else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    fclose(fp);

    if (mode == 'c')
        printf("Characters: %d\n", characters);
    else if (mode == 'w')
        printf("Words: %d\n", words);
    else if (mode == 'l')
        printf("Lines: %d\n", lines);
    else
        printf("Invalid count option. Use c/w/l.\n");
}

int main() {
    char input[MAX_CMD];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell$ ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = 0;

        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc == 0) continue;

        if (strcmp(args[0], "exit") == 0) break;

        if (strcmp(args[0], "count") == 0 && argc == 3) {
            pid_t pid = fork();
            if (pid == 0) {
                count_command(args[1][0], args[2]);
                exit(0);
            } else {
                wait(NULL);
            }
        } else {
            printf("Invalid command. Usage: count [c/w/l] filename\n");
        }
    }

    return 0;
}

