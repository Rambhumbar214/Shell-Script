

#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used;

    start = clock();  

    // Some sample computation (e.g., sum of squares)
    long long sum = 0;
    for (long long i = 0; i < 100000000; i++) {
        sum += i * i;
    }

    end = clock();  

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Sum: %lld\n", sum);
    printf("Execution Time: %f seconds\n", cpu_time_used);

    return 0;
}

