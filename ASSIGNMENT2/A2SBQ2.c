#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

struct Process {
    char pid[5];
    int arrival;
    int burst1;
    int burst2;
    int priority;
    int start1, end1, io_start, io_end, start2, end2;
    int wt, tat;
    bool completed;
};

int main() {
    struct Process p[MAX];
    int n, time = 0, completed = 0, i;
    int total_tat = 0, total_wt = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        sprintf(p[i].pid, "P%d", i);
        printf("Enter arrival time of %s: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("Enter first CPU burst of %s: ", p[i].pid);
        scanf("%d", &p[i].burst1);
        printf("Enter priority of %s (lower number = higher priority): ", p[i].pid);
        scanf("%d", &p[i].priority);
        p[i].burst2 = rand() % 8 + 3;
        p[i].completed = false;
    }

    printf("\nGantt Chart:\n");

    while (completed < n) {
        int idx = -1;
        int best_priority = 1e9;

        for (i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time && p[i].priority < best_priority) {
                best_priority = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].start1 = time;
        p[idx].end1 = time + p[idx].burst1;
        printf("%s_CPU1 [%d - %d]\n", p[idx].pid, p[idx].start1, p[idx].end1);
        time = p[idx].end1;

        p[idx].io_start = time;
        p[idx].io_end = time + 2;
        printf("%s_IO [%d - %d]\n", p[idx].pid, p[idx].io_start, p[idx].io_end);
        time = p[idx].io_end;

        p[idx].start2 = time;
        p[idx].end2 = time + p[idx].burst2;
        printf("%s_CPU2 [%d - %d]\n", p[idx].pid, p[idx].start2, p[idx].end2);
        time = p[idx].end2;

        p[idx].tat = p[idx].end2 - p[idx].arrival;
        p[idx].wt = p[idx].tat - (p[idx].burst1 + p[idx].burst2);
        p[idx].completed = true;
        completed++;
    }

    printf("\nProcess\tArrival\tBurst1\tBurst2\tPri\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst1, p[i].burst2, p[i].priority, p[i].tat, p[i].wt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    printf("\nAverage Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);

    return 0;
}

