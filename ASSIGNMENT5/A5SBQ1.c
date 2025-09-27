#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CHILDREN 5
#define MAX_NODES 10

typedef enum { OR_NODE, AND_NODE } NodeType;

typedef struct Node {
    char name[10];
    NodeType type;
    int child_count;
    struct Node* children[MAX_CHILDREN];
    int costs[MAX_CHILDREN];
    int solved;
    int value;
    struct Node* best_children[MAX_CHILDREN];
    int best_child_count;
} Node;

// Forward declaration
int ao_star(Node* node);

int ao_star(Node* node) {
    if (node->solved) return node->value;

    if (node->child_count == 0) {
        // Leaf node, cost assumed 0 here, can be customized
        node->value = 0;
        node->solved = 1;
        return node->value;
    }

    if (node->type == AND_NODE) {
        int total_cost = 0;
        for (int i = 0; i < node->child_count; i++) {
            int child_cost = node->costs[i] + ao_star(node->children[i]);
            total_cost += child_cost;
        }
        node->value = total_cost;

        // All children are in solution
        memcpy(node->best_children, node->children, sizeof(Node*) * node->child_count);
        node->best_child_count = node->child_count;
    }
    else { // OR_NODE
        int min_cost = INT_MAX;
        int best_idx = -1;
        for (int i = 0; i < node->child_count; i++) {
            int child_cost = node->costs[i] + ao_star(node->children[i]);
            if (child_cost < min_cost) {
                min_cost = child_cost;
                best_idx = i;
            }
        }
        node->value = min_cost;
        // Only one child chosen
        node->best_children[0] = node->children[best_idx];
        node->best_child_count = 1;
    }

    node->solved = 1;
    return node->value;
}

void print_solution(Node* node) {
    printf("%s ", node->name);
    if (node->best_child_count > 0) {
        printf("-> [ ");
        for (int i = 0; i < node->best_child_count; i++) {
            print_solution(node->best_children[i]);
        }
        printf("] ");
    }
}

int main() {
    // Create nodes
    Node A = {"A", OR_NODE, 2, {NULL}, {3, 2}, 0, INT_MAX, {NULL}, 0};
    Node B = {"B", AND_NODE, 2, {NULL}, {4, 5}, 0, INT_MAX, {NULL}, 0};
    Node C = {"C", OR_NODE, 0, {NULL}, {0}, 0, INT_MAX, {NULL}, 0};
    Node D = {"D", OR_NODE, 0, {NULL}, {0}, 0, INT_MAX, {NULL}, 0};
    Node E = {"E", OR_NODE, 0, {NULL}, {0}, 0, INT_MAX, {NULL}, 0};

    // Setup children references
    A.children[0] = &B;
    A.children[1] = &C;

    B.children[0] = &D;
    B.children[1] = &E;

    // Now run AO* from A
    int min_cost = ao_star(&A);
    printf("Minimum cost solution: %d\n", min_cost);
    printf("Solution path: ");
    print_solution(&A);
    printf("\n");

    return 0;
}

