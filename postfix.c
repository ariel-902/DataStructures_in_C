#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
const int MAX_LENGTH = 1000000;
const int MAX_NUMBER_LENGTH = 21;

struct stack
{
    char **data;  // an array as stack
    int lastItem; // last spot's index
};
void push(struct stack *inputStack, char *pushData)
{
    (*inputStack).lastItem++;
    strcpy((*inputStack).data[(*inputStack).lastItem], pushData);
}
void pop(struct stack *inputStack)
{
    for (int i = 0; i < MAX_NUMBER_LENGTH; i++)
        (*inputStack).data[(*inputStack).lastItem][i] = 0;
    (*inputStack).lastItem--;
}
char *peep(struct stack *inputStack)
{
    return (*inputStack).data[(*inputStack).lastItem];
}
int numberOrNot(char input)
{
    if (input >= 48 && input <= 57)
        return 1;
    else
        return 0;
}
char **preprocessing(char *input)
{
    char **infix = (char **)malloc(sizeof(char *) * MAX_LENGTH); // 二維動態
    for (int i = 0; i < MAX_LENGTH; i++)
        infix[i] = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
    for (int i = 0; i < MAX_LENGTH; i++)
        infix[i][0] = 0;
    int step = 0;
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        if (input[i] == 0)
            break;
        else
        {
            if (numberOrNot(input[i]) == 0) // operator
            {
                infix[step][0] = input[i];
                step++;
            }
            else //number
            {
                int temp = 0;
                while (numberOrNot(input[i]) != 0)
                {
                    infix[step][temp] = input[i];
                    temp++;
                    i++;
                }
                i--;
                step++;
            }
        }
    }
    return infix;
}
int priority(char *op)
{
    int pri = 0;
    if (op[0] == '+' || op[0] == '-')
        pri = 1;
    else if (op[0] == '*' || op[0] == '/' || op[0] == '%')
        pri = 2;
    else if (op[0] == '(')
        pri = 3;

    return pri;
}
long long floorMe(long long a_int, long long b_int)
{
    long long result = 0;
    if (a_int < 0)
        result = (a_int / b_int) - 1;
    else
        result = (a_int / b_int);
    return result;
}
char *calculateAB(char *a, char *b, char *op)
{
    char *ans = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
    long long a_int = atoll(a), b_int = atoll(b);
    if (op[0] == '+')
        sprintf(ans, "%lld", a_int + b_int);
    else if (op[0] == '-')
        sprintf(ans, "%lld", a_int - b_int);
    else if (op[0] == '*')
        sprintf(ans, "%lld", a_int * b_int);
    else if (op[0] == '/')
    {
        long long result = floorMe(a_int, b_int);
        sprintf(ans, "%lld", result);
    }
    else if (op[0] == '%')
        sprintf(ans, "%lld", a_int % b_int);

    return ans;
}
void calculate(char **postfix)
{
    struct stack calculateStack = {
        .data = (char **)malloc(sizeof(char *) * MAX_LENGTH),
        .lastItem = -1};
    for (int i = 0; i < MAX_LENGTH; i++)
        calculateStack.data[i] = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);

    for (int i = 0; i < MAX_LENGTH; i++)
    {
        // printf("%s", postfix[i]);
        if (postfix[i][0] == 0)
            break;
        else
        {
            if (numberOrNot(postfix[i][0]) == 1) // is a number
                push(&calculateStack, postfix[i]);
            else // is a operator
            {
                if (postfix[i][0] != '=')
                {
                    char *b = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                    strcpy(b, peep(&calculateStack));
                    pop(&calculateStack);

                    char *a = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                    strcpy(a, peep(&calculateStack));
                    pop(&calculateStack);

                    char *ans = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                    ans = calculateAB(a, b, postfix[i]);
                    push(&calculateStack, ans);
                    free(a);
                    free(b);
                    free(ans);
                }
                else
                {
                    char *output = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                    strcpy(output, peep(&calculateStack));
                    printf("Print: %s\n", output);
                    free(output);
                }
            }
        }
    }
}
char **translate(char **infix)
{
    struct stack translateStack = {// 先創建一個stack並初始化
                                   .data = (char **)malloc(sizeof(char *) * MAX_LENGTH),
                                   .lastItem = -1};
    for (int i = 0; i < MAX_LENGTH; i++)
        translateStack.data[i] = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);

    char **postfix = (char **)malloc(sizeof(char *) * MAX_LENGTH);
    for (int i = 0; i < MAX_LENGTH; i++)
        postfix[i] = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
    for (int i = 0; i < MAX_LENGTH; i++)
        postfix[i][0] = 0;
    int step = 0;
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        if (infix[i][0] == 0)
            break;
        else
        {
            if (numberOrNot(infix[i][0]) == 1) // is number
            {
                strcpy(postfix[step], infix[i]);
                step++;
            }
            else // is an operator
            {
                if (infix[i][0] == '(' || translateStack.lastItem == -1) // 為左括號或為第一個
                    push(&translateStack, infix[i]);
                else if (infix[i][0] == ')') //  若為右括號 則一直peep看看並pop直到遇到左括號為止
                {
                    char *preOperator = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                    strcpy(preOperator, peep(&translateStack));
                    while (preOperator[0] != '(')
                    {
                        pop(&translateStack);
                        strcpy(postfix[step], preOperator);
                        step++;
                        strcpy(preOperator, peep(&translateStack));
                    }
                    pop(&translateStack);
                    free(preOperator);
                }
                else
                {
                    if (infix[i][0] == '=')
                    {
                        while (translateStack.lastItem != -1)
                        {
                            char *opp = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                            strcpy(opp, peep(&translateStack));
                            strcpy(postfix[step], opp);
                            step++;
                            pop(&translateStack);
                            free(opp);
                        }
                        strcpy(postfix[step], infix[i]);
                        step++;
                    }
                    else
                    {
                        char *op = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
                        strcpy(op, peep(&translateStack));
                        int opPri, infPri;
                        opPri = priority(op);        //在stack中的operator
                        infPri = priority(infix[i]); //現在看到的operator
                        if (opPri != 3)
                        {
                            while (translateStack.lastItem != -1 && opPri >= infPri) // 不是空ㄉ
                            {
                                pop(&translateStack);
                                strcpy(postfix[step], op);
                                step++;
                                if (translateStack.lastItem != -1)
                                {
                                    strcpy(op, peep(&translateStack));
                                    opPri = priority(op);
                                }
                                if (opPri == 3)
                                    break;
                            }
                        }
                        push(&translateStack, infix[i]);
                        free(op);
                    }
                }
            }
        }
    }
    while (translateStack.lastItem != -1)
    {
        char *opp = (char *)malloc(sizeof(char) * MAX_NUMBER_LENGTH);
        strcpy(opp, peep(&translateStack));
        strcpy(postfix[step], opp);
        step++;
        pop(&translateStack);
        free(opp);
    }

    return postfix;
}
int main()
{
    char *input = (char *)malloc(sizeof(char) * MAX_LENGTH);
    for (int i = 0; i < MAX_LENGTH; i++)
        input[i] = 0;
    scanf("%s", input);
    char **infix;
    infix = preprocessing(input);

    char **postfix;
    postfix = translate(infix);
    calculate(postfix);

    free(input);
    free(infix);
    free(postfix);
}