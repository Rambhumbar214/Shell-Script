#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Process {
    char pid[5];
    int arrival;
    int burst1;
    int burst2;
    int tat;
    int wt;
};

int main() {
    struct Process p[MAX];
    int n, time = 0;
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        sprintf(p[i].pid, "P%d", i);
        printf("Enter arrival time of %s: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("Enter first CPU burst of %s: ", p[i].pid);
        scanf("%d", &p[i].burst1);
        p[i].burst2 = rand() % 8 + 3;
    }

    // Sort by arrival time
    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("\nGantt Chart:\n");

    for (i = 0; i < n; i++) {
        if (time < p[i].arrival) {
            printf("Idle [%d - %d]\n", time, p[i].arrival);
            time = p[i].arrival;
        }

        int start1 = time;
        time += p[i].burst1;
        printf("%s_CPU1 [%d - %d]\n", p[i].pid, start1, time);

        int io_start = time;
        time += 2;
        printf("%s_IO [%d - %d]\n", p[i].pid, io_start, time);

        int start2 = time;
        time += p[i].burst2;
        printf("%s_CPU2 [%d - %d]\n", p[i].pid, start2, time);

        p[i].tat = time - p[i].arrival;
        p[i].wt = p[i].tat - (p[i].burst1 + p[i].burst2);
    }

    int total_tat = 0, total_wt = 0;
    printf("\nProcess\tArrival\tBurst1\tBurst2\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst1, p[i].burst2, p[i].tat, p[i].wt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    printf("\nAverage Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);

    return 0;
}

