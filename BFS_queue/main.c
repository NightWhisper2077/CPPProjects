#include <stdio.h>
#include <stdlib.h>
#define WHITE 0
#define BLACK 1

typedef struct Vertex{
    int color;
    int d;
}Vertex;

typedef struct AdjList{
    int count;
    int reserve;
    int* adj;
}AdjList;

typedef struct Queue_List{
    int vert_ind;
    struct Queue_List* next;
}Queue;

typedef struct Head{
    Queue* next;
    Queue* last;
}Head;

void Push(int index, Head* head)
{
    Queue* a = (Queue*)malloc(sizeof(Queue));
    a->vert_ind = index;
    a->next = NULL;

    if (head->next == NULL)
    {
        head->next = a;
        head->last = a;
    }
    else
    {
        (head->last)->next = a;
        head->last = a;
    }
}

int Pop(Head* head)
{
    Queue* a = head->next;

    int index = a->vert_ind;
    head->next = a->next;

    free(a);
    return index;
}

void BFS(AdjList* a, Vertex* v, int count_vertices)
{
    v[1].d = 0;
    v[1].color = BLACK;

    Head* head = (Head*)malloc(sizeof(Head));
    head->next = NULL;
    head->last = NULL;
    Push(1, head);

    while (head->next != NULL)
    {
        int curVer = Pop(head);
        for (int i = 0; i<a[curVer].count; i++)
        {
            if (v[a[curVer].adj[i]].color == WHITE)
            {
                v[a[curVer].adj[i]].color = BLACK;
                v[a[curVer].adj[i]].d = v[curVer].d+1;
                Push(a[curVer].adj[i], head);
            }
        }
    }
}

void IncReserve(AdjList* a)
{
    if (a->count>=(a->reserve/2))
    {
        a->reserve *= 2;
        a->adj = (int*)realloc(a->adj, sizeof(int)*a->reserve);
    }
}

void AddAdj(AdjList* a, int to)
{
    a->adj[a->count] = to;
    a->count++;
    IncReserve(a);
}

int main()
{
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    int count_vertex, count_edge, from, to;
    fscanf(input, "%d %d", &count_vertex, &count_edge);

    Vertex* vertices = (Vertex*)malloc(sizeof(Vertex)*(count_vertex+1));
    AdjList* adjList = (AdjList*)malloc(sizeof(AdjList)*(count_vertex+1));

    for (int i = 0; i<=count_vertex; i++)
    {
        vertices[i].d = -1;
        vertices[i].color = WHITE;

        adjList[i].count = 0;
        adjList[i].reserve = 1;
        adjList[i].adj = (int*)malloc(sizeof(int)*adjList[i].reserve);
    }

    for (int i = 0; i<count_edge; i++)
    {
        fscanf(input, "%d %d", &from, &to);
        AddAdj(&adjList[from], to);
    }

    BFS(adjList, vertices, count_vertex);

    for (int i = 1; i<=count_vertex; i++)
    {
        fprintf(output,"%d\n", vertices[i].d);
    }
}