#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct disjointSet
{
    // TODO: Determine fields to use by your method
    int leaderIndex;
    int hashValue;
    int setSize;
} DisjointSet;

DisjointSet ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'z')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z')
        return c - 'A' + 36;
    return -1;
}

int hash(const char *s) // hash function
{
    int ret = 0;
    int mask = (1 << 24) - 1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char *s)
{
    // TODO: Initialize a set with hash value
    int i = hash(s);
    ds[i].hashValue = i;
    ds[i].leaderIndex = i; // 一開始的 leader 都是自己
    ds[i].setSize = 1;
}

inline void static init(const char *s)
{
    int i = hash(s);
    if (!set[i]) // if (s) has appeared
    {
        makeset(s);
        set[i] = 1;
    }
}

int find_set(const char *s)
{
    init(s);
    int i = hash(s);
    while (ds[i].leaderIndex != i)
        i = ds[i].leaderIndex;

    int j = hash(s); // path compression
    while (ds[j].leaderIndex != i)
    {
        int tmp = ds[j].leaderIndex;
        ds[j].leaderIndex = i;
        j = tmp;
    }
    return i;
}

void group(const char *ra, const char *rb) // union
{
    int a = find_set(ra), b = find_set(rb);
    if (a != b)
    {
        if (ds[a].setSize <= ds[b].setSize) // 更改 ds[a] 的
            ds[a].leaderIndex = b;
        else
            ds[b].leaderIndex = a;
        ds[a].setSize = ds[b].setSize + ds[a].setSize;
        ds[b].setSize = ds[a].setSize;
    }
}

bool same_set(const char *a, const char *b)
{
    int A = find_set(a), B = find_set(b);
    int Ahash = hash(a), Bhash = hash(b);
    bool ifA = set[Ahash], ifB = set[Bhash];
    if (ifA && ifB) // both already in disjoint set
    {
        if (A == B)
            return true;
        else
            return false;
    }
    else
        return false;
}

int main()
{
    // TODO: Implement your algorithm here
    int N;
    scanf("%d", &N);

    char op[6];
    char name1[13];
    char name2[13];
    for (int i = 0; i < N; i++)
    {
        scanf("%s %s %s", op, name1, name2);
        if (op[0] == 'g')
            group(name1, name2);
        else
        {
            bool same = same_set(name1, name2);
            if (same == true)
                printf("Positive\n");
            else
                printf("Negative\n");
        }
    }
}