#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define llu long long
int k, l, flag;
const llu MOD = LLONG_MAX / 1000;
typedef struct Node
{
    llu data;
    llu index;
} Node;
int cmpNode(const void *a, const void *b)
{
    const Node dataA = *(Node *)a;
    const Node dataB = *(Node *)b;
    llu d1 = dataA.data;
    llu d2 = dataB.data;
    if (d1 < d2)
        return -1;
    else if (d1 > d2)
        return 1;
    else
        return 0;
}
llu charToInt(char ch)
{
    llu num = ch - 33;
    return num;
}
void numbering(char **stringArray, llu **numberArray, llu **addArray, llu *sumArray) //  number the char O(kl)
{
    for (int i = 0; i < k; i++)
    {
        llu sum = 0, multiPower = 1;
        for (int j = l - 1; j >= 0; j--)
        {
            numberArray[i][j] = charToInt(stringArray[i][j]);
            sum += ((numberArray[i][j] * multiPower) % MOD);
            sum = sum % MOD;
            addArray[i][j] = ((93 - numberArray[i][j]) * multiPower) % MOD;
            multiPower = (multiPower * 94) % MOD;
        }
        sumArray[i] = sum;
    }
}
void matchingFlagZero(llu *sumArray, llu **addArray)
{
    int find = 0;
    Node *sortSum = (Node *)malloc(sizeof(Node) * k);

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < k; j++)
        {
            llu data = (sumArray[j] + addArray[j][i]) % MOD;
            llu index = j;
            Node node = {data, index};
            sortSum[j] = node;
        }
        qsort(sortSum, k, sizeof(Node), cmpNode); // sort k;
        for (int j = 0; j < k - 1; j++)
            if (sortSum[j].data == sortSum[j + 1].data)
            {
                printf("Yes\n");
                printf("%llu %llu", sortSum[j].index, sortSum[j + 1].index);
                find = 1;
                break;
            }
        if (find == 1)
            break;
    }
    if (find == 0)
        printf("No");
}
llu C(llu num1, llu num2)
{
    llu ans = (num1 * (num1 - 1)) / num2;
    return ans;
}
void matchingFlagOne(llu *sumArray, llu **addArray)
{
    // sort original rapin carp to find out the number of repeated string
    Node *tempSum = (Node *)malloc(sizeof(Node) * k);
    for (int i = 0; i < k; i++)
    {
        llu data = sumArray[i];
        llu index = i;
        Node node = {data, index};
        tempSum[i] = node;
    }
    qsort(tempSum, k, sizeof(Node), cmpNode);
    llu repeatNum = 0;
    for (int i = 0; i < k - 1; i++)
        if (tempSum[i].data == tempSum[i + 1].data)
        {
            llu repeatRange = 1;
            while (tempSum[i].data == tempSum[i + 1].data)
            {
                repeatRange++;
                i++;
                if (i >= k - 1)
                    break;
            }
            repeatNum += C(repeatRange, 2);
        }

    // mask every l and find the total pair num
    Node *sortSum = (Node *)malloc(sizeof(Node) * k);
    llu pairNum = 0;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < k; j++)
        {
            llu data = (sumArray[j] + addArray[j][i]) % MOD;
            llu index = j;
            Node node = {data, index};
            sortSum[j] = node;
        }
        qsort(sortSum, k, sizeof(Node), cmpNode); // sort k;
        for (int j = 0; j < k - 1; j++)
            if (sortSum[j].data == sortSum[j + 1].data)
            {
                llu range = 1;
                while (sortSum[j].data == sortSum[j + 1].data)
                {
                    range++;
                    j++;
                    if (j >= k - 1)
                        break;
                }
                pairNum += C(range, 2);
            }
    }

    pairNum -= (l - 1) * repeatNum;
    if (pairNum == 0)
        printf("No");
    else
        printf("Yes\n%llu", pairNum);
}
int main()
{
    scanf("%d %d %d", &k, &l, &flag);
    char **stringArray = (char **)malloc(sizeof(char *) * k);
    llu **numberArray = (llu **)malloc(sizeof(llu *) * k);
    llu **addArray = (llu **)malloc(sizeof(llu *) * k);
    llu *sumArray = (llu *)malloc(sizeof(llu) * k);
    for (int i = 0; i < k; i++)
    {
        stringArray[i] = (char *)malloc(sizeof(char) * (l + 1));
        numberArray[i] = (llu *)malloc(sizeof(llu) * l);
        addArray[i] = (llu *)malloc(sizeof(llu) * l);
        scanf("%s", stringArray[i]);
    }
    numbering(stringArray, numberArray, addArray, sumArray);
    if (flag == 0)
    {
        if (k != 1)
            matchingFlagZero(sumArray, addArray);
        else
            printf("No");
    }
    else if (flag == 1)
        matchingFlagOne(sumArray, addArray);
}