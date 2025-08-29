

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define MAX_CMD 1024
#define MAX_ARGS 10

void list_command(char mode, char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("Directory open failed");
        return;
    }

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (mode == 'f') {
            printf("%s\n", entry->d_name);
        } else if (mode == 'n') {
            count++;
        } else if (mode == 'i') {
            printf("%s\t(inode: %lu)\n", entry->d_name, entry->d_ino);
        } else {
            printf("Invalid list option. Use f/n/i.\n");
            closedir(dir);
            return;
        }
    }

    if (mode == 'n')
        printf("Total entries: %d\n", count);

    closedir(dir);
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

        if (strcmp(args[0], "list") == 0 && argc == 3) {
            list_command(args[1][0], args[2]);
        } else {
            printf("Invalid command. Usage: list [f/n/i] dirname\n");
        }
    }

    return 0;
}

