#include <stdio.h>
#include <stdbool.h>

#define P 5   // Number of processes
#define R 4   // Number of resources

void calculateNeed(int need[P][R], int max[P][R], int allocation[P][R]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

bool isSafe(int processes[], int avail[], int max[][R], int allot[][R]) {
    int need[P][R];
    calculateNeed(need, max, allot);

    bool finish[P] = {0};
    int safeSeq[P];
    int work[R];
    for (int i = 0; i < R; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > work[j])
                        break;
                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allot[p][k];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("System is NOT in safe state.\n");
            return false;
        }
    }

    printf("System is in safe state.\nSafe sequence is: ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return true;
}

bool requestResource(int processNum, int request[], int avail[], int max[][R], int allot[][R]) {
    int need[P][R];
    calculateNeed(need, max, allot);

    printf("Request by process P%d: ", processNum);
    for (int i = 0; i < R; i++) printf("%d ", request[i]);
    printf("\n");

    // Check if request <= need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[processNum][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Check if request <= available
    for (int i = 0; i < R; i++) {
        if (request[i] > avail[i]) {
            printf("Resources are not available currently.\n");
            return false;
        }
    }

    // Try allocating requested resources temporarily
    for (int i = 0; i < R; i++) {
        avail[i] -= request[i];
        allot[processNum][i] += request[i];
        max[processNum][i] -= request[i];  // Actually max stays same, keep as is or just recalc Need below
    }

    // Check system safety after allocation
    bool safe = isSafe(NULL, avail, max, allot);

    // Rollback the allocation changes (restore)
    for (int i = 0; i < R; i++) {
        avail[i] += request[i];
        allot[processNum][i] -= request[i];
        // max unchanged
    }

    if (safe) {
        printf("Request can be granted.\n");
    } else {
        printf("Request cannot be granted as it leads to unsafe state.\n");
    }
    return safe;
}

int main() {
    int processes[P] = {0,1,2,3,4};

    int allocation[P][R] = {
        {0,0,1,2},
        {1,0,0,0},
        {1,3,5,4},
        {0,6,3,2},
        {0,0,1,4}
    };

    int max[P][R] = {
        {0,0,1,2},
        {1,7,5,0},
        {2,3,5,6},
        {0,6,5,2},
        {0,6,5,6}
    };

    int available[R] = {1,5,2,0};

    int need[P][R];

    // a) Calculate and display Need matrix
    calculateNeed(need, max, allocation);
    printf("Need Matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < R; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // b) Check if system is in safe state and display safe sequence
    if (!isSafe(processes, available, max, allocation)) {
        // Unsafe state message already printed inside isSafe()
    }

    // c) Request from process P1 for (0,4,2,0)
    int requestP1[R] = {0,4,2,0};
    requestResource(1, requestP1, available, max, allocation);

    return 0;
}

