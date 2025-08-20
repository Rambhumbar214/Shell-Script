#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used;

    // Record start time
    start = clock();

    // === Set of instructions to measure ===
    long long sum = 0;
    for (long long i = 0; i < 100000000; i++) {
        sum += i;
    }
    printf("Sum = %lld\n", sum);
    // ======================================

    // Record end time
    end = clock();

    // Calculate time in seconds
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Execution time: %f seconds\n", cpu_time_used);

    return 0;
}

