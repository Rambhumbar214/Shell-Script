#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100
#define INF 100000

struct Process {
    char pid[5];
    int arrival;
    int burst1;
    int remaining;
    int burst2;
    int start_io;
    int end_io;
    int finish;
    int wt;
    int tat;
    bool io_done;
    bool second_burst_done;
    bool completed;
};

int main() {
    struct Process p[MAX];
    int n, time = 0, completed = 0;
    int total_tat = 0, total_wt = 0;
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        sprintf(p[i].pid, "P%d", i);
        printf("Enter arrival time of %s: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("Enter first CPU burst of %s: ", p[i].pid);
        scanf("%d", &p[i].burst1);
        p[i].remaining = p[i].burst1;
        p[i].burst2 = rand() % 8 + 3;
        p[i].io_done = false;
        p[i].second_burst_done = false;
        p[i].completed = false;
    }

    printf("\nGantt Chart:\n");

    while (completed < n) {
        int idx = -1;
        int min = INF;

        for (i = 0; i < n; i++) {
            if (!p[i].io_done && p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < min) {
                min = p[i].remaining;
                idx = i;
            }
        }

        if (idx != -1) {
            printf("%s [%d - %d]\n", p[idx].pid, time, time + 1);
            p[idx].remaining--;
            time++;

            if (p[idx].remaining == 0) {
                p[idx].start_io = time;
                p[idx].end_io = time + 2;
                p[idx].io_done = true;
                time = p[idx].end_io;
            }
        } else {
            time++;
        }

        for (i = 0; i < n; i++) {
            if (p[i].io_done && !p[i].second_burst_done && p[i].end_io <= time) {
                printf("%s_CPU2 [%d - %d]\n", p[i].pid, time, time + p[i].burst2);
                time += p[i].burst2;
                p[i].finish = time;
                p[i].tat = p[i].finish - p[i].arrival;
                p[i].wt = p[i].tat - (p[i].burst1 + p[i].burst2);
                p[i].second_burst_done = true;
                p[i].completed = true;
                completed++;
            }
        }
    }

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

