#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 9
#define BOARD_SIZE 3
#define MAX_QUEUE 100000

typedef struct Node {
    int board[SIZE];
    int g; // cost from start
    int h; // heuristic cost (misplaced tiles)
    int f; // total cost f = g + h
    struct Node* parent;
} Node;

typedef struct {
    Node* nodes[MAX_QUEUE];
    int size;
} PriorityQueue;

// Function prototypes
int is_goal(int* board, int* goal);
int heuristic(int* board, int* goal);
int boards_equal(int* b1, int* b2);
void swap(int* a, int* b);
void print_board(int* board);
void copy_board(int* src, int* dest);
int find_zero(int* board);
void push(PriorityQueue* pq, Node* node);
Node* pop(PriorityQueue* pq);
void heapify_up(PriorityQueue* pq, int index);
void heapify_down(PriorityQueue* pq, int index);
int contains(PriorityQueue* pq, int* board);
int is_in_closed(int closed[][SIZE], int closed_count, int* board);
void reconstruct_path(Node* node);

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main() {
    int start[SIZE] = {1, 2, 3,
                       4, 0, 5,
                       6, 7, 8};

    int goal[SIZE] = {1, 2, 3,
                      4, 5, 6,
                      7, 8, 0};

    PriorityQueue open_list;
    open_list.size = 0;

    int closed_list[100000][SIZE];
    int closed_count = 0;

    Node* start_node = (Node*)malloc(sizeof(Node));
    copy_board(start, start_node->board);
    start_node->g = 0;
    start_node->h = heuristic(start_node->board, goal);
    start_node->f = start_node->g + start_node->h;
    start_node->parent = NULL;

    push(&open_list, start_node);

    while (open_list.size > 0) {
        Node* current = pop(&open_list);

        if (is_goal(current->board, goal)) {
            printf("Solution found with cost %d:\n", current->g);
            reconstruct_path(current);
            // Free memory here if needed
            return 0;
        }

        // Add current to closed list
        copy_board(current->board, closed_list[closed_count]);
        closed_count++;

        int zero_pos = find_zero(current->board);
        int zx = zero_pos / BOARD_SIZE;
        int zy = zero_pos % BOARD_SIZE;

        for (int i = 0; i < 4; i++) {
            int nx = zx + dx[i];
            int ny = zy + dy[i];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                int new_board[SIZE];
                copy_board(current->board, new_board);
                // Swap zero with neighbor
                int new_pos = nx * BOARD_SIZE + ny;
                swap(&new_board[zero_pos], &new_board[new_pos]);

                // Check if new_board in closed list
                if (is_in_closed(closed_list, closed_count, new_board))
                    continue;

                Node* neighbor = (Node*)malloc(sizeof(Node));
                copy_board(new_board, neighbor->board);
                neighbor->g = current->g + 1;
                neighbor->h = heuristic(neighbor->board, goal);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                push(&open_list, neighbor);
            }
        }
    }

    printf("No solution found.\n");
    return 0;
}

// Check if current board matches goal
int is_goal(int* board, int* goal) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i] != goal[i])
            return 0;
    }
    return 1;
}

// Count number of misplaced tiles
int heuristic(int* board, int* goal) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        if (board[i] != 0 && board[i] != goal[i])
            count++;
    }
    return count;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void copy_board(int* src, int* dest) {
    memcpy(dest, src, SIZE * sizeof(int));
}

int find_zero(int* board) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i] == 0)
            return i;
    }
    return -1; // Should not happen
}

void print_board(int* board) {
    for (int i = 0; i < SIZE; i++) {
        if (i % BOARD_SIZE == 0) printf("\n");
        printf("%d ", board[i]);
    }
    printf("\n");
}

void reconstruct_path(Node* node) {
    if (node == NULL) return;
    reconstruct_path(node->parent);
    print_board(node->board);
    printf("\n");
}

// Priority Queue (Min-Heap) Operations
void push(PriorityQueue* pq, Node* node) {
    if (pq->size >= MAX_QUEUE) {
        printf("Priority queue overflow\n");
        return;
    }
    pq->nodes[pq->size] = node;
    heapify_up(pq, pq->size);
    pq->size++;
}

Node* pop(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    Node* top = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return top;
}

void heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->nodes[parent]->f <= pq->nodes[index]->f) break;

        Node* temp = pq->nodes[parent];
        pq->nodes[parent] = pq->nodes[index];
        pq->nodes[index] = temp;

        index = parent;
    }
}

void heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->nodes[left]->f < pq->nodes[smallest]->f)
        smallest = left;

    if (right < pq->size && pq->nodes[right]->f < pq->nodes[smallest]->f)
        smallest = right;

    if (smallest != index) {
        Node* temp = pq->nodes[smallest];
        pq->nodes[smallest] = pq->nodes[index];
        pq->nodes[index] = temp;
        heapify_down(pq, smallest);
    }
}

int is_in_closed(int closed[][SIZE], int closed_count, int* board) {
    for (int i = 0; i < closed_count; i++) {
        if (memcmp(closed[i], board, SIZE * sizeof(int)) == 0)
            return 1;
    }
    return 0;
}

