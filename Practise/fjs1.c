#include <stdio.h>

int main() {
    int bt[20], p[20], wt[20], tat[20], i, j, n, total = 0, pos, temp;
    float avg_wt, avg_tat;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("p%d: ", i + 1);
        scanf("%d", &bt[i]);
        p[i] = i + 1; // Store process number
    }

    // Sort processes based on burst time using Selection Sort
    for (i = 0; i < n; i++) {
        pos = i;
        for (j = i + 1; j < n; j++) {
            if (bt[j] < bt[pos]) {
                pos = j;
            }
        }
        // Swap burst times
        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;

        // Swap process numbers to maintain association
        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }

    // Calculate waiting time
    wt[0] = 0; // First process has 0 waiting time
    for (i = 1; i < n; i++) {
        wt[i] = 0;
        for (j = 0; j < i; j++) {
            wt[i] += bt[j]; // Sum of burst times of previously executed processes
        }
        total += wt[i];
    }
    avg_wt = (float)total / n;

    // Calculate turnaround time and total turnaround time
    total = 0; // Reset total for turnaround time calculation
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        total += tat[i];
        printf("p%d\t\t%d\t\t%d\t\t%d\n", p[i], bt[i], wt[i], tat[i]);
    }
    avg_tat = (float)total / n;

    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);

    return 0;
}
