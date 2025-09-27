#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // Needed for snprintf

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

void simulate_fifo(int reference_string[], int ref_len, int num_frames) {
    /**
     * Simulates the FIFO (First-In, First-Out) page replacement algorithm.
     * Uses a 'next_replacement_index' to track the oldest page (first-in).
     */
    if (num_frames <= 0) {
        printf("Error: The number of frames must be greater than zero.\n");
        return;
    }

    // Frames array to hold the pages in memory.
    // Initialized to -1 to signify an empty frame.
    int frames[num_frames];
    
    // This index points to the frame that will be replaced next (the 'oldest' entry)
    int next_replacement_index = 0;

    for (int j = 0; j < num_frames; j++) {
        frames[j] = -1;
    }

    int page_faults = 0;
    bool is_hit;

    printf("\n================================================================================\n");
    printf("SIMULATION: FIFO Page Replacement (Frames = %d)\n", num_frames);
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
                // FIFO does not update usage time on a hit.
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
                // Case 2: Frames are full - Perform FIFO replacement
                
                int victim_page = frames[next_replacement_index];
                
                // Replace the oldest page
                frames[next_replacement_index] = page;
                
                // Move the replacement pointer cyclically to the next oldest page
                next_replacement_index = (next_replacement_index + 1) % num_frames;

                snprintf(action, 40, "MISS (F) - Replaced %d (FIFO)", victim_page);
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
    // The new given page reference string for FIFO
    int reference_string[] = {3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6};
    // Calculate the length of the reference string array
    int ref_len = sizeof(reference_string) / sizeof(reference_string[0]);

    printf("Demand Paging Simulation - FIFO Algorithm (C Implementation)\n");
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

    // Call the FIFO simulation function
    simulate_fifo(reference_string, ref_len, num_frames);

    return 0;
}

