#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int N, M;
typedef struct Node
{
    int day;
    int a;
    int b;
    char instr;
    Node *parent;
} Node;
typedef struct Disjoint
{
    int leader;
    int setSize;
} Disjoint;
// ====== Disjoint Set's opertation ============
void makeSet(Disjoint **arr, int index)
{
    (*arr)[index].leader = index;
    (*arr)[index].setSize = 1;
}
void init(Disjoint **arr, int index, bool *set)
{
    if (set[index] == false)
    {
        makeSet(arr, index);
        set[index] = true;
    }
}
int findSet(Disjoint **arr, int index, bool *set)
{
    init(arr, index, set);
    int i = index;
    while ((*arr)[i].leader != i)
        i = (*arr)[i].leader;

    // path compression //
    int j = index;
    while ((*arr)[j].leader != i)
    {
        int tmp = (*arr)[j].leader;
        (*arr)[j].leader = i;
        j = tmp;
    }
    return i;
}
void merge(Disjoint **arr, int ra, int rb, int *shopNum, bool *set)
{
    int a = findSet(arr, ra, set), b = findSet(arr, rb, set);
    if (a != b)
    {
        if ((*arr)[a].setSize <= (*arr)[b].setSize) // union by size
            (*arr)[a].leader = b;
        else
            (*arr)[b].leader = a;
        (*arr)[a].setSize = (*arr)[b].setSize + (*arr)[a].setSize;
        (*arr)[b].setSize = (*arr)[a].setSize;
        (*shopNum)--;
    }
}
int main()
{
    scanf("%d %d\n", &N, &M);
    int shopNum = N;
    Disjoint *arr = (Disjoint *)malloc(sizeof(Disjoint) * (N + 1));

    char *situation = (char *)malloc(sizeof(char) * M);
    int *boomDays = (int *)malloc(sizeof(int) * M); // record boom k
    int **num = (int **)malloc(sizeof(int *) * M);
    for (int i = 0; i < M; i++)
    {
        int step = 0;
        num[i] = (int *)malloc(sizeof(int) * 2);
        char *tmp = malloc(sizeof(char) * 6);
        scanf("%s", tmp);
        if (tmp[0] == 'q')
        {
            situation[i] = 'q';
            num[i][0] = -1;
        }
        else if (tmp[0] == 'm')
        {
            int a, b = 0;
            situation[i] = 'm';
            scanf("%d %d", &a, &b);
            num[i][0] = a;
            num[i][1] = b;
        }
        else if (tmp[0] == 'b')
        {
            int k = 0;
            situation[i] = 'b';
            scanf("%d", &k);
            num[i][0] = k;
            boomDays[step] = k;
            step++;
        }
    }
    bool set[N + 1];
    for (int i = 0; i <= N; i++)
        set[i] = false;
    for (int i = 0; i < M; i++)
    {
        if (situation[i] == 'm')
            merge(&arr, num[i][0], num[i][1], &shopNum, set);
        else if (situation[i] == 'q')
            printf("%d\n", shopNum);
    }
}