#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 26  // A-Z max nodes
#define INF INT_MAX

typedef struct {
    int cost;      // edge cost
    int to;        // target node index
} Edge;

typedef struct {
    Edge edges[10];  // max 10 edges per node for demo
    int edge_count;
    int heuristic;   // h(n)
} Node;

typedef struct {
    int node;
    int g;  // cost from start to this node
    int f;  // f = g + h
    int parent;
} OpenNode;

typedef struct {
    OpenNode nodes[MAX_NODES];
    int count;
} OpenList;

void push(OpenList* openList, OpenNode node) {
    openList->nodes[openList->count++] = node;
}

int pop(OpenList* openList) {
    if (openList->count == 0) return -1;
    int bestIndex = 0;
    for (int i = 1; i < openList->count; i++) {
        if (openList->nodes[i].f < openList->nodes[bestIndex].f) {
            bestIndex = i;
        }
    }
    OpenNode bestNode = openList->nodes[bestIndex];
    // Remove best node from list
    for (int i = bestIndex; i < openList->count - 1; i++) {
        openList->nodes[i] = openList->nodes[i+1];
    }
    openList->count--;
    return bestNode.node;
}

// Helper to check if a node is in open list, returns index or -1
int in_open(OpenList* openList, int node) {
    for (int i = 0; i < openList->count; i++) {
        if (openList->nodes[i].node == node)
            return i;
    }
    return -1;
}

// Reconstruct path using parent array
void reconstruct_path(int parents[], int start, int goal) {
    if (goal == start) {
        printf("%c ", start + 'A');
        return;
    }
    reconstruct_path(parents, start, parents[goal]);
    printf("-> %c ", goal + 'A');
}

int main() {
    // Map nodes A-J to indices 0-9
    // Define graph nodes with heuristic values
    Node graph[MAX_NODES];

    // Initialize graph edges and heuristics
    // For example: A (0) connected to B(1) cost 1, C(2) cost 4
    // Fill edges accordingly for your graph

    // Clear
    for(int i=0; i<MAX_NODES; i++) {
        graph[i].edge_count = 0;
        graph[i].heuristic = 0;
    }

    // Sample graph edges and heuristics (replace with your graph)
    // A
    graph[0].edges[0] = (Edge){1, 1}; // A->B cost 1
    graph[0].edges[1] = (Edge){4, 2}; // A->C cost 4
    graph[0].edge_count = 2;
    graph[0].heuristic = 7;

    // B
    graph[1].edges[0] = (Edge){2, 3}; // B->D cost 2
    graph[1].edges[1] = (Edge){5, 4}; // B->E cost 5
    graph[1].edge_count = 2;
    graph[1].heuristic = 6;

    // C
    graph[2].edges[0] = (Edge){3, 5}; // C->F cost 3
    graph[2].edge_count = 1;
    graph[2].heuristic = 5;

    // D
    graph[3].edges[0] = (Edge){1, 6}; // D->G cost 1
    graph[3].edge_count = 1;
    graph[3].heuristic = 4;

    // E
    graph[4].edges[0] = (Edge){2, 6}; // E->G cost 2
    graph[4].edges[1] = (Edge){1, 7}; // E->H cost 1
    graph[4].edge_count = 2;
    graph[4].heuristic = 3;

    // F
    graph[5].edges[0] = (Edge){4, 7}; // F->H cost 4
    graph[5].edge_count = 1;
    graph[5].heuristic = 6;

    // G
    graph[6].edges[0] = (Edge){2, 8}; // G->I cost 2
    graph[6].edge_count = 1;
    graph[6].heuristic = 2;

    // H
    graph[7].edges[0] = (Edge){2, 8}; // H->I cost 2
    graph[7].edges[1] = (Edge){5, 9}; // H->J cost 5
    graph[7].edge_count = 2;
    graph[7].heuristic = 1;

    // I
    graph[8].edges[0] = (Edge){1, 9}; // I->J cost 1
    graph[8].edge_count = 1;
    graph[8].heuristic = 1;

    // J
    graph[9].edge_count = 0;
    graph[9].heuristic = 0;

    int start = 0; // A
    int goal = 9;  // J

    OpenList openList;
    openList.count = 0;

    int closed[MAX_NODES] = {0};
    int g_scores[MAX_NODES];
    int parents[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        g_scores[i] = INF;
        parents[i] = -1;
    }

    g_scores[start] = 0;
    push(&openList, (OpenNode){start, 0, graph[start].heuristic, -1});

    while (openList.count > 0) {
        // Get node with smallest f
        int current_index = 0;
        int min_f = INF;
        for (int i = 0; i < openList.count; i++) {
            if (openList.nodes[i].f < min_f) {
                min_f = openList.nodes[i].f;
                current_index = i;
            }
        }

        OpenNode current = openList.nodes[current_index];

        // Remove current from openList
        for (int i = current_index; i < openList.count - 1; i++) {
            openList.nodes[i] = openList.nodes[i + 1];
        }
        openList.count--;

        if (current.node == goal) {
            printf("Path found with cost %d:\n", current.g);
            reconstruct_path(parents, start, goal);
            printf("\n");
            return 0;
        }

        closed[current.node] = 1;

        // Explore neighbors
        for (int i = 0; i < graph[current.node].edge_count; i++) {
            Edge e = graph[current.node].edges[i];
            if (closed[e.to]) continue;

            int tentative_g = g_scores[current.node] + e.cost;
            if (tentative_g < g_scores[e.to]) {
                g_scores[e.to] = tentative_g;
                parents[e.to] = current.node;

                int index_in_open = in_open(&openList, e.to);
                int f = tentative_g + graph[e.to].heuristic;
                if (index_in_open == -1) {
                    push(&openList, (OpenNode){e.to, tentative_g, f, current.node});
                } else {
                    // Update node in open list if better path found
                    if (openList.nodes[index_in_open].f > f) {
                        openList.nodes[index_in_open].g = tentative_g;
                        openList.nodes[index_in_open].f = f;
                        openList.nodes[index_in_open].parent = current.node;
                    }
                }
            }
        }
    }

    printf("No path found from %c to %c\n", start + 'A', goal + 'A');
    return 0;
}

