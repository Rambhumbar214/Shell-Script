#include <stdio.h>
#include <stdbool.h>

#define P 5  // number of processes
#define R 3  // number of resource types

int main() {
    // Given data:

    int Allocation[P][R] = {
        {0, 1, 0},  // P0
        {2, 0, 0},  // P1
        {3, 0, 3},  // P2
        {2, 1, 1},  // P3
        {0, 0, 2}   // P4
    };

    int Request[P][R] = {
        {0, 0, 0},  // P0
        {2, 0, 2},  // P1
        {0, 0, 1},  // P2
        {1, 0, 0},  // P3
        {0, 0, 2}   // P4
    };

    int TotalResources[R] = {7, 2, 6}; // Total instances of A, B, C

    int Available[R];

    // Calculate Available = TotalResources - sum(Allocation)
    for (int j = 0; j < R; j++) {
        int sum = 0;
        for (int i = 0; i < P; i++) {
            sum += Allocation[i][j];
        }
        Available[j] = TotalResources[j] - sum;
    }

    // Print Available array
    printf("Available resources: ");
    for (int j = 0; j < R; j++) {
        printf("%d ", Available[j]);
    }
    printf("\n");

    bool Finish[P] = {false}; // Track if process can finish
    bool progress;

    do {
        progress = false;
        for (int i = 0; i < P; i++) {
            if (!Finish[i]) {
                // Check if Request[i] <= Available
                bool can_allocate = true;
                for (int j = 0; j < R; j++) {
                    if (Request[i][j] > Available[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    // Process can finish, release its Allocation
                    for (int j = 0; j < R; j++) {
                        Available[j] += Allocation[i][j];
                    }
                    Finish[i] = true;
                    progress = true;
                    printf("Process P%d has finished and released resources.\n", i);
                }
            }
        }
    } while (progress);

    // Check for deadlock
    bool deadlock = false;
    printf("\nDeadlocked processes: ");
    for (int i = 0; i < P; i++) {
        if (!Finish[i]) {
            deadlock = true;
            printf("P%d ", i);
        }
    }

    if (!deadlock) {
        printf("None\nNo deadlock detected.");
    } else {
        printf("\nDeadlock detected!\n");
    }

    return 0;
}

