#include <stdio.h>
#include "bignumber.h"
#include <stdlib.h>
#include <strings.h>

char *readDynamicLine(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    int c;
    size_t index = 0;

    while ((c = getchar()) != EOF && c != '\n')
    {
        if (index == len)
        {
            len += 10;
            line = realloc(line, len);
            if (line == NULL)
            {
                fprintf(stderr, "Erro na alocacao de memoria\n");
                exit(EXIT_FAILURE);
            }
        }
        line[index++] = (char)c;
    }

    if (index == 0 && c == EOF)
    {
        free(line);
        return NULL;
    }

    line[index] = '\0';

    return line;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Informe o nome do arquivo.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Problema ao abrir arquivo.\n");
        return 1;
    }

    BigNumber *num1 = createBigNumber();
    BigNumber *num2 = createBigNumber();
    BigNumber *result = createBigNumber();
    char operation, *strNum1, *strNum2;

    strNum1 = readDynamicLine();
    strNum2 = readDynamicLine();
    scanf(" %c", &operation);

    setBigNumberFromString(num1, strNum1);
    setBigNumberFromString(num2, strNum2);

    switch (operation)
    {
    case '+':
        addBigNumbers(num1, num2, result);
        break;
    case '-':
        subBigNumbers(num1, num2, result);
        break;
    case '*':
        multiplyBigNumbers(num1, num2, result);
        break;
    default:
        fprintf(stderr, "Operacao nao suportada: %c\n", operation);
        break;
    }

    printf("%s\n", result->digits);

    free(strNum1);
    free(strNum2);
    destroyBigNumber(num1);
    destroyBigNumber(num2);
    destroyBigNumber(result);

    return 0;
}