#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define XOR(a, b) (node *)((uintptr_t)(a) ^ (uintptr_t)(b))
const int MAX_INPUT_LENGTH = 12;
// ========= doubly linked lists' functions ========
typedef struct node
{
    struct node *link; // xor of next and previous
    int id;
    int line;
    int group;
} node;
int isEmpty(node *head)
{
    int empty = 0;
    if (head == NULL)
        empty = 1;
    return empty;
}
node *createNode(node *nextAddr, node *preAddr, int group, int id, int line)
{
    node *Node = (node *)malloc(sizeof(node));
    Node->link = XOR(nextAddr, preAddr);
    Node->group = group;
    Node->id = id;
    Node->line = line;

    return Node;
}
int checkGroup(int **bathgroup, int checkbath, int checkgroup)
{
    int haveOrNot = 0;
    if (bathgroup[checkbath][checkgroup] > 0)
        haveOrNot = 1;
    return haveOrNot;
}
void insertLast(node **head, node **tail, int **bathgroup, int InsertGroup, int InsertID, int InsertLine)
{
    // update bathgroup
    bathgroup[InsertLine][InsertGroup]++;

    // insert
    if (isEmpty(*head) == 0) // is not empty
    {
        node *insertNode = createNode(NULL, (*tail), InsertGroup, InsertID, InsertLine);
        (*tail)->link = XOR((*tail)->link, insertNode);
        (*tail) = insertNode;
    }
    else // is empty
    {
        node *insertNode = createNode(NULL, NULL, InsertGroup, InsertID, InsertLine);
        (*head) = insertNode;
        (*tail) = insertNode;
    }
}
void deleteLast(node **head, node **tail, int tarLine, int **order, int **bathgroup, int groupNum, int tarOrder, int *firstMinusOne)
{
    // update bathgroup
    bathgroup[tarLine][(*tail)->group]--;

    // delete
    if (NULL != XOR(NULL, (*tail)->link))
    {
        node *preNodeAddr = XOR(NULL, (*tail)->link);
        preNodeAddr->link = XOR(preNodeAddr->link, (*tail));
        (*tail) = preNodeAddr;
    }
    else // only one node
    {
        order[tarLine][tarOrder] = -1;
        firstMinusOne[tarLine]--;
        if (firstMinusOne[tarLine] < 0)
            firstMinusOne[tarLine] += groupNum;
        (*head) = NULL;
        (*tail) = NULL;
    }
}
void deleteFirst(node **head, node **tail, int tarLine, int **bathgroup)
{
    // update bathgroup
    bathgroup[tarLine][(*head)->group]--;

    // delete
    if (NULL != XOR(NULL, (*head)->link))
    {
        node *nextNodeAddr = XOR(NULL, (*head)->link);
        nextNodeAddr->link = XOR((*head), nextNodeAddr->link);
        (*head) = nextNodeAddr;
    }
    else // only one node
    {
        (*head) = NULL;
        (*tail) = NULL;
    }
}
void reverse(node **head, node **tail) // swap head and tail
{
    node *tmp;
    tmp = (*head);
    (*head) = (*tail);
    (*tail) = tmp;
}
// ========= problems' funtions ======================
void enter(node ***bathHead, node ***bathTail, int **bathgroup, int **order, int groupNum, int InsertGroup, int InsertID, int InsertLine, int *firstMinusOne)
{
    // update order array
    if (checkGroup(bathgroup, InsertLine, InsertGroup) == 0) //  若原本沒有
    {
        order[InsertLine][firstMinusOne[InsertLine]] = InsertGroup;
        firstMinusOne[InsertLine]++;
        if (firstMinusOne[InsertLine] >= groupNum)
            firstMinusOne[InsertLine] -= groupNum;
    }
    // insert
    insertLast(&bathHead[InsertLine][InsertGroup], &bathTail[InsertLine][InsertGroup], bathgroup, InsertGroup, InsertID, InsertLine);
}
void leave(node **head, node **tail, int tarLine, int **order, int **bathgroup, int groupNum, int tarOrder, int *firstMinusOne)
{
    deleteLast(head, tail, tarLine, order, bathgroup, groupNum, tarOrder, firstMinusOne);
}
void go(node **head, node **tail, int tarLine, int **bathgroup)
{
    deleteFirst(head, tail, tarLine, bathgroup);
}
void close(int **bathgroup, int **order, node ***bathHead, node ***bathTail, int groupNum, int closeLine, int *firstMinusOne, int *firstOrder, int *frontOpen, int *backOpen)
{

    int nearestIndex = frontOpen[closeLine];
    // update frontOpen & backOpen
    frontOpen[backOpen[closeLine]] = frontOpen[closeLine];
    backOpen[frontOpen[closeLine]] = backOpen[closeLine];

    // close;
    int end = firstMinusOne[closeLine];
    int start = firstOrder[closeLine];
    if (end <= start)
        end += groupNum;
    for (int i = end - 1; i >= start; i--)
    {
        int index = i;
        if (i >= groupNum)
            index -= groupNum;
        int tar = order[closeLine][index];
        if (tar == -1)
            continue;
        if (bathHead[closeLine][tar] != NULL && bathHead[nearestIndex][tar] == NULL)
        {
            reverse(&bathHead[closeLine][tar], &bathTail[closeLine][tar]);
            order[nearestIndex][firstMinusOne[nearestIndex]] = tar;
            firstMinusOne[nearestIndex]++;
            if (firstMinusOne[nearestIndex] >= groupNum)
                firstMinusOne[nearestIndex] -= groupNum;
            bathHead[nearestIndex][tar] = bathHead[closeLine][tar];
            bathTail[nearestIndex][tar] = bathTail[closeLine][tar];
            bathHead[closeLine][tar] = NULL;
            bathTail[closeLine][tar] = NULL;
            bathgroup[nearestIndex][tar] += bathgroup[closeLine][tar];
            bathgroup[closeLine][tar] = 0;
        }
        else if (bathHead[closeLine][tar] != NULL && bathHead[nearestIndex][tar] != NULL) // 兩邊都有
        {
            reverse(&bathHead[closeLine][tar], &bathTail[closeLine][tar]);
            bathTail[nearestIndex][tar]->link = XOR(bathTail[nearestIndex][tar]->link, bathHead[closeLine][tar]);
            bathHead[closeLine][tar]->link = XOR(bathTail[nearestIndex][tar], bathHead[closeLine][tar]->link);
            bathTail[nearestIndex][tar] = bathTail[closeLine][tar];
            bathHead[closeLine][tar] = NULL;
            bathTail[closeLine][tar] = NULL;
            bathgroup[nearestIndex][tar] += bathgroup[closeLine][tar];
            bathgroup[closeLine][tar] = 0;
        }
    }
}
int main()
{
    int bathNum = 0, situaNum = 0, groupNum = 0;
    scanf("%d %d %d", &bathNum, &situaNum, &groupNum);

    // build and initialize
    node ***bathHead = (node ***)malloc(sizeof(node **) * bathNum);
    node ***bathTail = (node ***)malloc(sizeof(node **) * bathNum);
    int *frontOpen = (int *)malloc(sizeof(int) * bathNum);
    int *backOpen = (int *)malloc(sizeof(int) * bathNum);
    int *firstMinusOne = (int *)malloc(sizeof(int) * bathNum);
    int *firstOrder = (int *)malloc(sizeof(int) * bathNum);
    int **bathgroup = (int **)malloc(sizeof(int *) * bathNum);
    int **order = (int **)malloc(sizeof(int *) * bathNum);
    for (int i = 0; i < bathNum; i++)
    {
        bathHead[i] = (node **)malloc(sizeof(node *) * groupNum);
        bathTail[i] = (node **)malloc(sizeof(node *) * groupNum);
        bathgroup[i] = (int *)malloc(sizeof(int) * groupNum);
        order[i] = (int *)malloc(sizeof(int) * groupNum);
        firstMinusOne[i] = 0;
        firstOrder[i] = 0;
        frontOpen[i] = i - 1;
        backOpen[i] = i + 1;
        for (int j = 0; j < groupNum; j++)
        {
            bathHead[i][j] = NULL;
            bathTail[i][j] = NULL;
            bathgroup[i][j] = 0;
            order[i][j] = -1;
        }
    }
    frontOpen[0] = bathNum - 1;
    backOpen[bathNum - 1] = 0;

    char *instruction = (char *)malloc(sizeof(char) * MAX_INPUT_LENGTH);
    for (int i = 0; i < situaNum; i++)
    {
        scanf("%s", instruction);
        if (strcmp(instruction, "enter") == 0) // if is enter
        {
            int InsertGroup = 0, InsertID = 0, InsertLine = 0;
            scanf("%d %d %d", &InsertGroup, &InsertID, &InsertLine);
            enter(bathHead, bathTail, bathgroup, order, groupNum, InsertGroup, InsertID, InsertLine, firstMinusOne);
        }
        else if (strcmp(instruction, "leave") == 0)
        {
            int leaveLine = 0;
            scanf("%d", &leaveLine);
            int target = 0;
            int tarOrder = 0;
            if (firstMinusOne[leaveLine] == 0)
            {
                target = order[leaveLine][groupNum - 1];
                tarOrder = groupNum - 1;
            }
            else
            {
                target = order[leaveLine][firstMinusOne[leaveLine] - 1];
                tarOrder = firstMinusOne[leaveLine] - 1;
            }
            leave(&bathHead[leaveLine][target], &bathTail[leaveLine][target], leaveLine, order, bathgroup, groupNum, tarOrder, firstMinusOne);
        }
        else if (strcmp(instruction, "go") == 0)
        {
            int leaveLine = 0;
            scanf("%d", &leaveLine);
            go(&bathHead[leaveLine][order[leaveLine][firstOrder[leaveLine]]], &bathTail[leaveLine][order[leaveLine][firstOrder[leaveLine]]], leaveLine, bathgroup);
            if (bathHead[leaveLine][order[leaveLine][firstOrder[leaveLine]]] == NULL)
            {
                firstOrder[leaveLine]++;
                if (firstOrder[leaveLine] >= groupNum)
                    firstOrder[leaveLine] -= groupNum;
            }
        }
        else if (strcmp(instruction, "close") == 0)
        {
            int closeLine = 0;
            scanf("%d", &closeLine);
            close(bathgroup, order, bathHead, bathTail, groupNum, closeLine, firstMinusOne, firstOrder, frontOpen, backOpen);
        }
    }
    //printf
    for (int i = 0; i < bathNum; i++)
    {
        int end = firstMinusOne[i];
        int start = firstOrder[i];
        if (end <= start)
            end += groupNum;
        for (int j = start; j < end; j++)
        {
            int index = j;
            if (j >= groupNum)
                index -= groupNum;
            if (order[i][index] != -1)
            {
                node *tmpHead = bathHead[i][order[i][index]];
                if (tmpHead == NULL)
                    break;
                else
                {
                    node *pre = NULL;
                    node *tmp = NULL;
                    node *cur = tmpHead;
                    while (cur != NULL)
                    {
                        printf("%d", cur->id);
                        tmp = cur;
                        cur = XOR(pre, cur->link);
                        pre = tmp;
                        if (cur != NULL)
                            printf(" ");
                    }
                    printf(" ");
                }
            }
            else
                break;
        }
        printf("\n");
    }
}
