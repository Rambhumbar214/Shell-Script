#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 

// Helper function to print the current state of the memory frames
void print_frames(int frames[], int num_frames) {
    printf("| ");
    for (int j = 0; j < num_frames; j++) {
        if (frames[j] == -1) {
            printf("  | "); // Use empty space for an unfilled slot
        } else {
            printf("%d | ", frames[j]);
        }
    }
}

// Helper function to find the index of the page in frames that won't be used for the longest time
int find_optimal_victim_index(int frames[], int num_frames, int reference_string[], int ref_len, int current_index) {
    int victim_index = 0;
    // max_future_use_index stores the index in the reference string of the furthest future use.
    // Initialized to -1 to ensure the first frame is considered.
    int max_future_use_index = -1; 

    for (int j = 0; j < num_frames; j++) {
        int page_in_frame = frames[j];
        // next_use_index is initialized to 'infinity' (end of string)
        int next_use_index = ref_len; 

        // Search for the next appearance of this page in the future
        for (int k = current_index + 1; k < ref_len; k++) {
            if (reference_string[k] == page_in_frame) {
                next_use_index = k;
                break; // Found its next use
            }
        }

        // If a page will not be used again (next_use_index == ref_len), it is the optimal choice.
        if (next_use_index == ref_len) {
            return j; // Return this frame index immediately
        }

        // Check if this page is further out than the current maximum
        if (next_use_index > max_future_use_index) {
            max_future_use_index = next_use_index;
            victim_index = j;
        }
    }
    return victim_index;
}

void simulate_opt(int reference_string[], int ref_len, int num_frames) {
    /**
     * Simulates the OPTIMAL page replacement algorithm.
     * Looks ahead in the reference string to find the page that will be
     * needed furthest in the future (or never again).
     */
    if (num_frames <= 0) {
        printf("Error: The number of frames must be greater than zero.\n");
        return;
    }

    // Frames array to hold the pages in memory.
    // Initialized to -1 to signify an empty frame.
    int frames[num_frames];
    
    for (int j = 0; j < num_frames; j++) {
        frames[j] = -1;
    }

    int page_faults = 0;
    bool is_hit;

    printf("\n================================================================================\n");
    printf("SIMULATION: OPTIMAL Page Replacement (Frames = %d)\n", num_frames);
    printf("================================================================================\n");
    printf("%-5s%-10s%-30s%-20s\n", "Step", "Page Ref", "Memory Frames", "Event");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < ref_len; i++) {
        int page = reference_string[i];
        is_hit = false;
        char action[40]; 
        snprintf(action, 40, "HIT (H)");

        // 1. Check for a Page Hit
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                is_hit = true;
                break;
            }
        }

        // 2. Handle Page Fault (MISS)
        if (!is_hit) {
            page_faults++;
            
            // Determine if there is a free slot (denoted by -1)
            int free_slot_index = -1;
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == -1) {
                    free_slot_index = j;
                    break;
                }
            }

            if (free_slot_index != -1) {
                // Case 1: Free slot available
                frames[free_slot_index] = page;
                snprintf(action, 40, "MISS (F) - Free Slot");
            } else {
                // Case 2: Frames are full - Perform OPTIMAL replacement
                
                // Find the index of the optimal victim page
                int victim_index = find_optimal_victim_index(frames, num_frames, reference_string, ref_len, i);
                
                int victim_page = frames[victim_index];
                
                // Replace the victim page
                frames[victim_index] = page;

                snprintf(action, 40, "MISS (F) - Replaced %d (OPT)", victim_page);
            }
        }

        // 3. Display the current step result
        printf("%-5d%-10d", i + 1, page);
        print_frames(frames, num_frames);
        printf("%-20s\n", action);
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("\nTotal Page References: %d\n", ref_len);
    printf("Total Number of Frames: %d\n", num_frames);
    printf("FINAL RESULT: Total Page Faults = %d\n", page_faults);
    printf("================================================================================\n");
}

int main() {
    // The new given page reference string for OPTIMAL
    int reference_string[] = {12, 15, 12, 18, 6, 8, 11, 12, 19, 12, 6, 8, 12, 15, 19, 8};
    // Calculate the length of the reference string array
    int ref_len = sizeof(reference_string) / sizeof(reference_string[0]);

    printf("Demand Paging Simulation - OPTIMAL Algorithm (C Implementation)\n");
    printf("Reference String: ");
    for (int i = 0; i < ref_len; i++) {
        printf("%d%s", reference_string[i], (i == ref_len - 1) ? "" : ", ");
    }
    printf("\n");

    int num_frames;
    while (true) {
        printf("Enter the number of memory frames (n): ");
        if (scanf("%d", &num_frames) == 1 && num_frames > 0) {
            break;
        } else {
            printf("Invalid input. Please enter a positive integer for the number of frames.\n");
            // Clear input buffer in case of non-integer input
            while (getchar() != '\n');
        }
    }

    // Call the OPTIMAL simulation function
    simulate_opt(reference_string, ref_len, num_frames);

    return 0;
}

