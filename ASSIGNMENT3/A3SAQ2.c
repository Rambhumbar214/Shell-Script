#include <stdio.h>

int main() {
    int i, j, process, resource;

    printf("Enter the number of processes: ");
    scanf("%d", &process);

    printf("Enter the number of resources: ");
    scanf("%d", &resource);

    int max[process][resource];
    int allocation[process][resource];
    int need[process][resource];

    // Input Max matrix
    printf("Enter the Max matrix:\n");
    for (i = 0; i < process; i++) {
        for (j = 0; j < resource; j++) {
            printf("Max[%d][%d]: ", i, j);
            scanf("%d", &max[i][j]);
        }
    }

    // Input Allocation matrix
    printf("Enter the Allocation matrix:\n");
    for (i = 0; i < process; i++) {
        for (j = 0; j < resource; j++) {
            printf("Allocation[%d][%d]: ", i, j);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate Need matrix
    for (i = 0; i < process; i++) {
        for (j = 0; j < resource; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Display Need matrix
    printf("\nNeed Matrix:\n");
    for (i = 0; i < process; i++) {
        for (j = 0; j < resource; j++) {
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }

    return 0;
}

