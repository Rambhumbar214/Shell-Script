#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

struct Process {
    char pid[5];
    int arrival;
    int burst1;
    int burst2;
    int tat;
    int wt;
    bool completed;
};

int main() {
    struct Process p[MAX];
    int n, time = 0;
    int i, completed_count = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        sprintf(p[i].pid, "P%d", i);
        printf("Enter arrival time of %s: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("Enter first CPU burst of %s: ", p[i].pid);
        scanf("%d", &p[i].burst1);
        p[i].burst2 = rand() % 8 + 3;
        p[i].completed = false;
    }

    printf("\nGantt Chart:\n");

    while (completed_count < n) {
        int idx = -1;
        int min_burst = 1e9;

        for (i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time && p[i].burst1 < min_burst) {
                min_burst = p[i].burst1;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        int start1 = time;
        time += p[idx].burst1;
        printf("%s_CPU1 [%d - %d]\n", p[idx].pid, start1, time);

        int io_start = time;
        time += 2;
        printf("%s_IO [%d - %d]\n", p[idx].pid, io_start, time);

        int start2 = time;
        time += p[idx].burst2;
        printf("%s_CPU2 [%d - %d]\n", p[idx].pid, start2, time);

        p[idx].tat = time - p[idx].arrival;
        p[idx].wt = p[idx].tat - (p[idx].burst1 + p[idx].burst2);
        p[idx].completed = true;
        completed_count++;
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

