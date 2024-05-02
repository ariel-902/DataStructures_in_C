#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "price.h"
#include <limits.h>
#define ll unsigned long long
const int MAX_CAPACITY = 100000000;
// heap function (use an array to implement)
typedef struct minHeap
{
    ll *heapArray;
    int curHeapSize;
    int capacity;
} minHeap;
minHeap *createHeap(int size)
{
    minHeap *heap = (minHeap *)malloc(sizeof(minHeap));
    heap->heapArray = (ll *)malloc(sizeof(ll) * size);
    heap->curHeapSize = 0;
    heap->capacity = size;
    for (int i = 0; i < size; i++)
        heap->heapArray[i] = -1;

    return heap;
}
void swap(ll *num1, ll *num2)
{
    ll tmp;
    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}
void insert(minHeap **heap, ll insertPrice)
{
    (*heap)->curHeapSize++;
    ll *arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    int cursor = tail;
    int parent = tail / 2;
    arr[tail] = insertPrice;
    while (arr[cursor] < arr[parent] && cursor != 1)
        if (arr[cursor] < arr[parent])
        {
            swap(&arr[cursor], &arr[parent]);
            // update cursor
            cursor = parent;
            parent = cursor / 2;
        }
}
void heapify(minHeap **heap, int target)
{
    ll *arr = (*heap)->heapArray;
    int left = target * 2;
    int right = target * 2 + 1;
    int len = (*heap)->curHeapSize;
    int min = 0;
    // three are all in heap
    if (right <= len)
        while (true)
        {
            // choose the MIN
            if (arr[target] <= arr[left] && arr[target] <= arr[right])
                min = target;
            else if (arr[left] <= arr[right] && arr[left] <= arr[target])
                min = left;
            else if (arr[right] <= arr[left] && arr[right] <= arr[target])
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
            if (right > (*heap)->curHeapSize)
                break;
        }
    if (left <= len && right > len)
    {
        if (arr[target] > arr[left])
            swap(&arr[target], &arr[left]);
    }
}
void extractMin(minHeap **heap)
{
    ll *arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    swap(&arr[1], &arr[tail]);
    (*heap)->curHeapSize--;
    heapify(heap, 1);
}
int extractMinWithReturn(minHeap **heap)
{
    ll *arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    swap(&arr[1], &arr[tail]);
    (*heap)->curHeapSize--;
    heapify(heap, 1);
    return arr[tail];
}
void heapSort(minHeap **heap)
{
    ll *arr = (*heap)->heapArray;
    int tail = (*heap)->curHeapSize;
    for (int i = tail; i > 1; i--)
        extractMin(heap);
}
// operation function
void buildStockHeap(ll *stock, int N, int A, int K, minHeap **heap)
{
    for (int i = 0; i < A; i++)
        for (ll j = 1; j <= N + K; j++)
            insert(heap, price(stock[i], j));
}
ll decideK(minHeap **heap, int K, int tail)
{
    return (*heap)->heapArray[tail - K + 1];
}
int main()
{
    int A = 0, Q = 0, N = 0;
    scanf("%d %d %d", &A, &Q, &N);

    ll *stock = (ll *)malloc(sizeof(ll) * A);
    int count = 0;
    do
    {
        scanf("%llu", &stock[count++]);
    } while (getchar() != '\n' && count < 100);

    int **query = (int **)malloc(sizeof(int *) * Q);
    int max_K = 0;
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
        ll *t_arr = (ll *)malloc(sizeof(ll) * A); // 紀錄每個stock現在走到第幾天
        ll *result = (ll *)malloc(sizeof(ll) * (max_K + 10));
        minHeap *heap = createHeap(A + 10);

        // insert first element
        int step = 0;
        ll minPrice = ULLONG_MAX;
        int minStockIndex = 0;
        for (int i = 0; i < A; i++) // initialize t_arr
        {
            t_arr[i] = 1;
            int tmpPrice = price(stock[i], t_arr[i]);
            insert(&heap, tmpPrice);
            if (tmpPrice < minPrice)
            {
                minPrice = tmpPrice;
                minStockIndex = i;
            }
        }
        result[step] = extractMinWithReturn(&heap);
        t_arr[minStockIndex]++;
        step++;
        // int left = heap->heapArray[1];
        // int right = heap->heapArray[2];

        while (step <= max_K)
        {
            insert(&heap, price(stock[minStockIndex], t_arr[minStockIndex]));
            minPrice = extractMinWithReturn(&heap);
            for (int i = 0; i < A; i++)
            {
                if (price(stock[i], t_arr[i]) == minPrice)
                {
                    minStockIndex = i;
                    break;
                }
            }
            result[step] = minPrice;
            t_arr[minStockIndex]++;
            step++;
            if (step == max_K + 1)
                break;
        }
        for (int i = 0; i < Q; i++)
            printf("%llu\n", result[query[i][1] - 1]);
    }
    else //  other subtasks
    {
        minHeap *heap = createHeap(MAX_CAPACITY);
        buildStockHeap(stock, N, A, max_K, &heap);
        int tail = 0;
        for (int i = 1; i < MAX_CAPACITY; i++)
            if ((heap)->heapArray[i] != -1)
                tail++;
            else
                break;
        heapSort(&heap);
        for (int i = 0; i < Q; i++)
            printf("%llu\n", decideK(&heap, query[i][1], tail));
    }
}