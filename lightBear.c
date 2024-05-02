#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
typedef struct node
{
    struct node *next;
    int index;
} node;
node *createNode(node *nextAddr, int index)
{
    node *Node = (node *)malloc(sizeof(node));
    Node->next = nextAddr;
    Node->index = index;

    return Node;
}
void destroy(node *head)
{
    if (head != NULL)
    {
        destroy(head->next);
        free(head);
    }
}
void insertLast(node **head, node **tail, int index)
{
    // insert
    if (*head != NULL) // is not empty
    {
        node *insertNode = createNode(NULL, index);
        (*tail)->next = insertNode;
        (*tail) = insertNode;
    }
    else // is empty
    {
        node *insertNode = createNode(NULL, index);
        (*head) = insertNode;
        (*tail) = insertNode;
    }
}
void addAdj(node **a_head, node **a_tail, node **b_head, node **b_tail, int a_index, int b_index)
{
    insertLast(a_head, a_tail, b_index);
    insertLast(b_head, b_tail, a_index);
}
void Euler(int **EulerArray, node **adjList_head, int resort, int *resortIndex, int startIndex, int startDepth, int *step, int *visit)
{
    visit[startIndex] = true;
    if (startIndex == resort)
        *resortIndex = *step;
    EulerArray[(*step)][0] = startIndex;
    EulerArray[(*step)][1] = startDepth;
    (*step)++;
    node *cur = adjList_head[startIndex];
    while (cur != NULL)
    {
        int curIndex = cur->index;
        if (visit[curIndex] == false)
        {
            Euler(EulerArray, adjList_head, resort, resortIndex, curIndex, startDepth + 1, step, visit);
            EulerArray[(*step)][0] = startIndex;
            EulerArray[(*step)][1] = startDepth;
            (*step)++;
        }
        cur = cur->next;
    }
}
void query(int **EulerArray, int captial, int resortIndex, int *queryArray, int cityNum)
{
    // go front half
    int minDepth = EulerArray[resortIndex][1];
    int minCity = EulerArray[resortIndex][0];
    for (int i = resortIndex; i >= 0; i--)
    {
        if (EulerArray[i][1] < minDepth)
        {
            minDepth = EulerArray[i][1];
            minCity = EulerArray[i][0];
        }
        EulerArray[i][2] = minDepth;
        EulerArray[i][3] = minCity;
    }

    minDepth = EulerArray[resortIndex][1];
    minCity = EulerArray[resortIndex][0];
    // go back half
    for (int i = resortIndex; i < 2 * cityNum - 1; i++)
    {
        if (EulerArray[i][1] < minDepth)
        {
            minDepth = EulerArray[i][1];
            minCity = EulerArray[i][0];
        }
        EulerArray[i][2] = minDepth;
        EulerArray[i][3] = minCity;
    }

    for (int i = 0; i < 2 * cityNum - 1; i++)
        queryArray[EulerArray[i][0]] = EulerArray[i][3];
}
int main()
{
    int cityNum, queryNum, capital, resort = 0;
    int i = 0;
    scanf("%d %d %d %d", &cityNum, &queryNum, &capital, &resort);

    node **adjList_head = (node **)malloc(sizeof(node *) * cityNum);
    node **adjList_tail = (node **)malloc(sizeof(node *) * cityNum);
    int *visit = (int *)malloc(sizeof(int) * cityNum);
    int *queryArray = (int *)malloc(sizeof(int) * cityNum);
    for (i = 0; i < cityNum; i++)
    {
        adjList_head[i] = NULL;
        adjList_tail[i] = NULL;
        visit[i] = 0;
        queryArray[i] = 0;
    }
    int **EulerArray = (int **)malloc(sizeof(int *) * (2 * cityNum - 1));
    for (i = 0; i < 2 * cityNum - 1; i++)
        EulerArray[i] = (int *)malloc(sizeof(int) * 4);

    for (i = 0; i < 2 * cityNum - 1; i++)
        for (int j = 0; j < 4; j++)
            EulerArray[i][j] = 0;

    int a_index, b_index;
    for (i = 0; i < cityNum - 1; i++)
    {
        scanf("%d %d", &a_index, &b_index);
        addAdj(&adjList_head[a_index - 1], &adjList_tail[a_index - 1], &adjList_head[b_index - 1], &adjList_tail[b_index - 1], a_index - 1, b_index - 1);
    }

    int step = 0;
    int resortIndex = 0;
    Euler(EulerArray, adjList_head, resort - 1, &resortIndex, capital - 1, 0, &step, visit);
    query(EulerArray, capital - 1, resortIndex, queryArray, cityNum);

    int queryCity = 0;
    for (i = 0; i < queryNum; i++)
    {
        scanf("%d", &queryCity);
        printf("%d\n", queryArray[queryCity - 1] + 1);
    }
}
