#include <stdio.h>
#include <limits.h> // For INT_MAX

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
};

int main() {
    // ... (Input process details)

    int current_time = 0;
    int completed_processes = 0;
    int num_processes;
    while (completed_processes < num_processes) {
        int shortest_job_index = -1;
        int min_remaining_time = INT_MAX;

        // Find the process with the shortest remaining burst time
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && 
                processes[i].is_completed == 0 &&
                processes[i].remaining_burst_time < min_remaining_time) {
                min_remaining_time = processes[i].remaining_burst_time;
                shortest_job_index = i;
            }
        }

        if (shortest_job_index == -1) { // CPU idle
            current_time++;
        } else {
            processes[shortest_job_index].remaining_burst_time--;
            current_time++;

            if (processes[shortest_job_index].remaining_burst_time == 0) {
                processes[shortest_job_index].is_completed = 1;
                completed_processes++;
                processes[shortest_job_index].completion_time = current_time;
                processes[shortest_job_index].turnaround_time = 
                    processes[shortest_job_index].completion_time - processes[shortest_job_index].arrival_time;
                processes[shortest_job_index].waiting_time = 
                    processes[shortest_job_index].turnaround_time - processes[shortest_job_index].burst_time;
            }
        }
    }

    // ... (Calculate and display average times)
    return 0;
}
