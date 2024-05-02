#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "price.h"
#include <limits.h>
#define ll unsigned long long
const ll MAX_CAPACITY = 10000000;
const ll Sub4_MAX_CAPACITY = 1000000000000;
// heap function (use an array to implement)
typedef struct Node
{
    ll price;
    int stockIndex;
} Node;
typedef struct minHeap
{
    Node **heapArray;
    int curHeapSize;
    int capacity;
} minHeap;
Node *createNode(ll price, int stockIndex)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->price = price;
    node->stockIndex = stockIndex;

    return node;
}
minHeap *createHeap(ll size)
{
    minHeap *heap = (minHeap *)malloc(sizeof(minHeap));
    heap->heapArray = (Node **)malloc(sizeof(Node *) * size);
    heap->curHeapSize = 0;
    heap->capacity = size;
    for (int i = 0; i < size; i++)
        heap->heapArray[i] = NULL;

    return heap;
}
void swap(Node **num1, Node **num2)
{
    Node *tmp;
    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}
void insert(minHeap **heap, ll insertPrice, int stockIndex)
{
    (*heap)->curHeapSize++;
    Node **arr = (*heap)->heapArray;
    Node *node = createNode(insertPrice, stockIndex);
    int tail = (*heap)->curHeapSize;
    int cursor = tail;
    int parent = tail / 2;
    arr[tail] = node;
    if (parent > 0)
        while (arr[cursor]->price < arr[parent]->price && cursor > 1)
        {
            swap(&arr[cursor], &arr[parent]);
            // update cursor
            cursor = parent;
            parent = cursor / 2;
            if (parent == 0)
                break;
        }
}
void heapify(minHeap **heap, int target)
{
    Node **arr = (*heap)->heapArray;
    int left = target * 2;
    int right = target * 2 + 1;
    int len = (*heap)->curHeapSize;
    int min = 0;
    // three are all in heap
    if (right <= len)
        while (true)
        {
            // choose the MIN
            if (arr[target]->price <= arr[left]->price && arr[target]->price <= arr[right]->price)
                min = target;
            else if (arr[left]->price <= arr[right]->price && arr[left]->price <= arr[target]->price)
                min = left;
            else if (arr[right]->price <= arr[left]->price && arr[right]->price <= arr[target]->price)
                min = right;

            // swapping process
            if (min == target)
                break;
            else
            {
                swap(&arr[target], &arr[min]);
                target = min;
                left = target * 2;
                right = target * 2 + 1;
            }
            if (right > len)
                break;
        }
    if (left == len && right > len)
        if (arr[target]->price > arr[left]->price)
            swap(&arr[target], &arr[left]);
}
void extractMin(minHeap **heap)
{
    Node **arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    swap(&arr[1], &arr[tail]);
    (*heap)->curHeapSize--;
    heapify(heap, 1);
}
Node *extractMinWithReturn(minHeap **heap)
{
    Node **arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    swap(&arr[1], &arr[tail]);
    (*heap)->curHeapSize--;
    heapify(heap, 1);
    return arr[tail];
}
void heapSort(minHeap **heap)
{
    Node **arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    for (int i = tail; i > 1; i--)
        extractMin(heap);
}
// operation function
void buildStockHeap(ll *stock, int N, int A, int K, minHeap **heap)
{
    for (int i = 0; i < A; i++)
        for (ll j = 1; j <= N + K; j++)
            insert(heap, price(stock[i], j), i);
}
ll decideK(minHeap **heap, int K, int tail)
{
    return (*heap)->heapArray[tail - K + 1]->price;
}
int main()
{
    int A = 0, Q = 0, N = 0;
    scanf("%d %d %d", &A, &Q, &N);

    ll *stock = (ll *)malloc(sizeof(ll) * A);
    // int count = 0;
    // do
    // {
    //     scanf("%llu", &stock[count++]);
    // } while (getchar() != '\n' && count < 100);

    for (int i = 0; i < A; i++)
        scanf("%llu", &stock[i]);

    int **query = (int **)malloc(sizeof(int *) * Q);
    ll max_K = 0;
    int zero = 1;
    for (int i = 0; i < Q; i++)
    {
        query[i] = (int *)malloc(sizeof(int) * 2);
        query[i][0] = 0;
        query[i][1] = 0;
        scanf("%d %d", &query[i][0], &query[i][1]);
        if (query[i][1] >= max_K)
            max_K = query[i][1];
        if (query[i][0] != 0)
            zero = 0;
    }

    if (A > 1 && N == 1 && zero == 1) // subtask 2
    {
        ll *t_arr = (ll *)malloc(sizeof(ll) * A);
        ll *result = (ll *)malloc(sizeof(ll) * (max_K + 10));
        minHeap *heap = createHeap(A + 10);

        // insert first element
        int step = 0;
        for (int i = 0; i < A; i++) // initialize t_arr
        {
            t_arr[i] = 1;
            ll tmpPrice = price(stock[i], t_arr[i]);
            insert(&heap, tmpPrice, i);
        }
        Node *returnNode = extractMinWithReturn(&heap);
        result[step] = returnNode->price;
        t_arr[returnNode->stockIndex]++;
        step++;

        while (step <= max_K)
        {
            insert(&heap, price(stock[returnNode->stockIndex], t_arr[returnNode->stockIndex]), returnNode->stockIndex);
            returnNode = extractMinWithReturn(&heap);
            result[step] = returnNode->price;
            t_arr[returnNode->stockIndex]++;
            step++;
            if (step == max_K + 1)
                break;
        }
        for (int i = 0; i < Q; i++)
            printf("%llu\n", result[query[i][1] - 1]);
    }
    else if (zero == 1 && N > 1 && A > 1) // subtask4
    {
        minHeap *heap = createHeap(Sub4_MAX_CAPACITY);
        buildStockHeap(stock, N, A, max_K, &heap);
        int tail = 0;
        for (int i = 1; i <= Sub4_MAX_CAPACITY; i++)
            if ((heap)->heapArray[i] != NULL)
                tail++;
            else
                break;
        heapSort(&heap);
        for (int i = 0; i < Q; i++)
            printf("%llu\n", decideK(&heap, query[i][1], tail));
    }
    else //  other subtasks
    {
        minHeap *heap = createHeap(MAX_CAPACITY);
        buildStockHeap(stock, N, A, max_K, &heap);
        int tail = 0;
        for (int i = 1; i <= MAX_CAPACITY; i++)
            if ((heap)->heapArray[i] != NULL)
                tail++;
            else
                break;
        heapSort(&heap);
        for (int i = 0; i < Q; i++)
            printf("%llu\n", decideK(&heap, query[i][1], tail));
    }
}