#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
const int MAX_LENGHT = 10000000 + 1;
void orginAddRev(char *input, char *addRev, char *revAddRev, int inputLen) // O(N)
{
    int j = inputLen;
    addRev[j] = '*';
    revAddRev[j] = '*';
    for (int i = 0; i < inputLen; i++)
    {
        addRev[i] = input[i];
        addRev[j + 1] = input[(inputLen - 1) - i];
        revAddRev[i] = input[(inputLen - 1) - i];
        revAddRev[j + 1] = input[i];
        j++;
    }
    addRev[2 * inputLen + 1] = 0;
    revAddRev[2 * inputLen + 1] = 0;
}
int *calculatePi(char *addRev, int *pi) //O(N) 可以從一半開始算就好
{
    pi[0] = -1;
    int k = -1;
    int piLen = strlen(addRev);
    int maxPi = 0, maxIndex = 0;
    for (int q = 1; q < piLen; q++)
    {
        while (k > -1 && addRev[k + 1] != addRev[q])
            k = pi[k];
        if (addRev[k + 1] == addRev[q])
            k++;
        pi[q] = k;
        if (q >= piLen / 2)
            if (pi[q] >= maxPi)
            {
                maxPi = pi[q];
                maxIndex = q;
            }
    }

    int *maxInfo = (int *)malloc(sizeof(int) * 2);
    maxInfo[0] = maxIndex;
    maxInfo[1] = maxPi;

    return maxInfo;
}
int calculatePalindrome(char *addRev, int *pi, int *maxInfo, int inputLen) //O(N)
{
    int finalIndex = maxInfo[0] - (maxInfo[1] + 1);
    int ans = finalIndex - inputLen;
    addRev[finalIndex + 1] = 0;

    return ans;
}
int main()
{
    char *input = (char *)malloc(sizeof(char) * MAX_LENGHT);
    scanf("%s", input);

    int inputLen = strlen(input);
    char *addRev = (char *)malloc(sizeof(char) * (2 * inputLen + 2));
    char *revaddRev = (char *)malloc(sizeof(char) * (2 * inputLen + 2));
    orginAddRev(input, addRev, revaddRev, inputLen);
    int *pi = (int *)malloc(sizeof(int) * (2 * inputLen + 1));
    int *maxInfo = calculatePi(addRev, pi);
    int ans = calculatePalindrome(addRev, pi, maxInfo, inputLen);
    int *maxInfoRev = calculatePi(revaddRev, pi);
    int ansRev = calculatePalindrome(revaddRev, pi, maxInfoRev, inputLen);

    // print the result
    if (maxInfo[1] > maxInfoRev[1])
    {
        printf("%d\n", ans);
        printf("%s", &addRev[inputLen + 1]);
        addRev[inputLen] = 0;
        printf("%s", addRev);
    }
    else if (maxInfo[1] < maxInfoRev[1])
    {
        printf("%d\n", ansRev);
        printf("%s", &revaddRev[inputLen + 1]);
        revaddRev[inputLen] = 0;
        printf("%s", revaddRev);
    }
    else if (maxInfo[1] == maxInfoRev[1])
    {
        printf("%d\n", ans);
        if (ans != 0)
        {
            printf("%s", &addRev[inputLen + 1]);
            addRev[inputLen] = 0;
            printf("%s\n", addRev);
            printf("%s", &revaddRev[inputLen + 1]);
            revaddRev[inputLen] = 0;
            printf("%s", revaddRev);
        }
        else
            printf("%s", input);
    }
}