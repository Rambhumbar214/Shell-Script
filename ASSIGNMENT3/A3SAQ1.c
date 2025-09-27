#include <stdio.h>

int main() {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int min_resources = n - 1;

    printf("Minimum number of resources required to avoid deadlock: %d\n", min_resources);

    return 0;
}

