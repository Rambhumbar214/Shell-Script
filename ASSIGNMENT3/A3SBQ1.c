#include <stdio.h>
#define MAX 10

int allocation[MAX][MAX], max[MAX][MAX], need[MAX][MAX];
int available[MAX];
int process, resource;

void acceptAvailable() {
    printf("Enter Available resources:\n");
    for (int i = 0; i < resource; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i]);
    }
}

void acceptMatrices() {
    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < process; i++) {
        for (int j = 0; j < resource; j++) {
            printf("Allocation[%d][%d]: ", i, j);
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("Enter Max Matrix:\n");
    for (int i = 0; i < process; i++) {
        for (int j = 0; j < resource; j++) {
            printf("Max[%d][%d]: ", i, j);
            scanf("%d", &max[i][j]);
        }
    }
}

void displayMatrix(const char* name, int matrix[MAX][MAX]) {
    printf("%s Matrix:\n", name);
    for (int i = 0; i < process; i++) {
        for (int j = 0; j < resource; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void computeAndDisplayNeed() {
    printf("Need Matrix:\n");
    for (int i = 0; i < process; i++) {
        for (int j = 0; j < resource; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }
}

void displayAvailable() {
    printf("Available Resources:\n");
    for (int i = 0; i < resource; i++) {
        printf("R%d: %d\n", i, available[i]);
    }
}

int main() {
    int choice;

    printf("Enter number of processes: ");
    scanf("%d", &process);

    printf("Enter number of resources: ");
    scanf("%d", &resource);

    acceptAvailable();
    acceptMatrices();

    do {
        printf("\n--- Menu ---\n");
        printf("1. Display Allocation Matrix\n");
        printf("2. Display Max Matrix\n");
        printf("3. Compute and Display Need Matrix\n");
        printf("4. Display Available Resources\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayMatrix("Allocation", allocation);
                break;
            case 2:
                displayMatrix("Max", max);
                break;
            case 3:
                computeAndDisplayNeed();
                break;
            case 4:
                displayAvailable();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}

