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

void simulate_mru(int reference_string[], int ref_len, int num_frames) {
    /**
     * Simulates the MRU (Most Recently Used) page replacement algorithm.
     * Uses a 'last_used' array to track the last access time (step number)
     * for each page currently in a frame. The page with the highest last_used time is replaced.
     */
    if (num_frames <= 0) {
        printf("Error: The number of frames must be greater than zero.\n");
        return;
    }

    // Frames array to hold the pages in memory.
    // Initialized to -1 to signify an empty frame.
    int frames[num_frames];
    // last_used array stores the step index (i+1) when the corresponding page
    // in 'frames' was last accessed. Used to find the MRU victim.
    int last_used[num_frames];
    
    for (int j = 0; j < num_frames; j++) {
        frames[j] = -1;
        last_used[j] = 0; // Initialize usage time to 0
    }

    int page_faults = 0;
    bool is_hit;

    printf("\n================================================================================\n");
    printf("SIMULATION: MRU Page Replacement (Frames = %d)\n", num_frames);
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
                // Update usage time: this page is now the MOST recently used
                last_used[j] = i + 1; 
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
                last_used[free_slot_index] = i + 1; // Mark as most recently used
                snprintf(action, 40, "MISS (F) - Free Slot");
            } else {
                // Case 2: Frames are full - Perform MRU replacement
                int mru_index = 0;
                int max_last_used_time = last_used[0];

                // Find the index of the page that was used the most recently (maximum last_used time)
                for (int j = 1; j < num_frames; j++) {
                    if (last_used[j] > max_last_used_time) {
                        max_last_used_time = last_used[j];
                        mru_index = j;
                    }
                }
                
                int victim_page = frames[mru_index];
                
                // Replace the MRU page
                frames[mru_index] = page;
                last_used[mru_index] = i + 1; // Mark new page as most recently used

                snprintf(action, 40, "MISS (F) - Replaced %d (MRU)", victim_page);
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
    // The given page reference string for MRU
    int reference_string[] = {2, 5, 2, 8, 5, 4, 1, 2, 3, 2, 6, 1, 2, 5, 9, 8};
    // Calculate the length of the reference string array
    int ref_len = sizeof(reference_string) / sizeof(reference_string[0]);

    printf("Demand Paging Simulation - MRU Algorithm (C Implementation)\n");
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

    // Call the MRU simulation function
    simulate_mru(reference_string, ref_len, num_frames);

    return 0;
}

