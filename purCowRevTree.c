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
    int change; // meaningful only for merge
    int unchange;
    int branch;
    char instr;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;
typedef struct Disjoint
{
    int leader;
    int setSize;
} Disjoint;
// ====== Node's operation =========
Node *createNode(int day, int a, int b, char instr)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->day = day;
    node->a = a;
    node->b = b;
    node->instr = instr;
    node->change = 0;

    return node;
}
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
    return i;
}
void merge(Disjoint **arr, Node **node, int *shopNum, bool *set)
{
    int ra = (*node)->a, rb = (*node)->b;
    int a = findSet(arr, ra, set), b = findSet(arr, rb, set);
    if (a != b)
    {
        if ((*arr)[a].setSize <= (*arr)[b].setSize) // union by size
        {
            (*arr)[a].leader = b;
            (*node)->change = a;
            (*node)->unchange = b;
            (*arr)[b].setSize = (*arr)[a].setSize + (*arr)[b].setSize;
        }
        else
        {
            (*arr)[b].leader = a;
            (*node)->change = b;
            (*node)->unchange = a;
            (*arr)[a].setSize = (*arr)[a].setSize + (*arr)[b].setSize;
        }
        // (*arr)[a].setSize = (*arr)[b].setSize + (*arr)[a].setSize;
        // (*arr)[b].setSize = (*arr)[a].setSize;
        (*shopNum)--;
    }
}
void undo(Disjoint **arr, int change, int unchange, int *shopNum)
{
    if (change != 0)
    {
        (*arr)[change].leader = change;
        (*arr)[unchange].setSize -= (*arr)[change].setSize;
        (*shopNum)++;
    }
}
void dfs(Disjoint **arr, Node **root, int *shopNum, int *queryAns, bool *set)
{
    if ((*root) != NULL)
    {
        if ((*root)->instr == 'm')
            merge(arr, root, shopNum, set);
        else if ((*root)->instr == 'q')
            queryAns[(*root)->day] = (*shopNum);
        dfs(arr, &((*root)->right), shopNum, queryAns, set);
        dfs(arr, &((*root)->left), shopNum, queryAns, set);
        if ((*root)->instr == 'm')
            undo(arr, (*root)->change, (*root)->unchange, shopNum);
    }
}
int main()
{
    scanf("%d %d\n", &N, &M);
    int shopNum = N;
    Disjoint *arr = (Disjoint *)malloc(sizeof(Disjoint) * (N + 1));
    Node **address = (Node **)malloc(sizeof(Node *) * (M + 1));
    Node **branchTail = (Node **)malloc(sizeof(Node *) * (M + 1));
    int *queryAns = (int *)malloc(sizeof(int) * (M + 1));
    Node *root = createNode(0, -1, -1, 'r');
    Node *tail = root;
    root->parent = NULL;
    root->left = NULL;
    root->right = NULL;
    root->branch = 0;
    branchTail[0] = root;
    address[0] = root;
    for (int i = 0; i < M; i++)
    {
        queryAns[i] = -1;
        Node *node;
        char *tmp = malloc(sizeof(char) * 6);
        int k = 0;
        scanf("%s", tmp);
        if (tmp[0] != 'b')
        {
            if (tmp[0] == 'q')
                node = createNode(i + 1, -1, -1, 'q');
            else if (tmp[0] == 'm')
            {
                int a, b = 0;
                scanf("%d %d", &a, &b);
                node = createNode(i + 1, a, b, 'm');
            }
            tail->right = node;
            tail->right->parent = tail;
            tail->right->right = NULL;
            tail->right->left = NULL;
            tail->right->branch = i + 1;
            address[i + 1] = tail->right;
            branchTail[i + 1] = tail->right;
            tail = tail->right;
        }
        else // boom
        {
            scanf("%d", &k);
            Node *boomNode = createNode(i + 1, k, -1, 'b');
            Node *tarNode = branchTail[address[k]->branch];
            tarNode->left = boomNode;
            tarNode->left->parent = tarNode;
            tarNode->left->right = NULL;
            tarNode->left->left = NULL;
            tarNode->left->branch = tarNode->branch;
            branchTail[address[k]->branch] = tarNode->left;
            address[i + 1] = tarNode->left;
            tail = boomNode;
        }
    }

    queryAns[M] = -1;
    bool set[N + 1];
    for (int i = 0; i <= N; i++)
        set[i] = false;
    dfs(&arr, &root, &shopNum, queryAns, set);
    for (int i = 1; i <= M; i++)
        if (queryAns[i] != -1)
            printf("%d\n", queryAns[i]);
    // Node *cur = root->right->left->left;
    // // // printf("%c\n", cur->left->instr);
    // // // printf("%d\n", cur->left->day);
    // while (cur->right != NULL)
    // {
    //     cur = cur->right;
    //     printf("%c\n", cur->instr);
    // }
}