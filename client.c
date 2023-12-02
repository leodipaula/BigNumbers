#include <stdio.h>
#include "bignumber.h"
#include <stdlib.h>
#include <strings.h>

// Função para a leitura dinâmica de uma linha com tratamento de erros
char *readDynamicLine()
{
    char *line = NULL;
    size_t len = 0;
    int c;
    size_t index = 0;

    while ((c = getchar()) != EOF && c != '\n')
    {
        if (index == len)
        {
            len += 10; // Aumentar o tamanho do buffer, se necessário
            line = realloc(line, len);
            if (line == NULL)
            {
                fprintf(stderr, "Erro na alocação de memória\n");
                exit(EXIT_FAILURE);
            }
        }
        line[index++] = (char)c;
    }

    if (index == 0 && c == EOF)
    {
        // Nenhum caractere lido e encontramos EOF
        free(line);
        return NULL;
    }

    line[index] = '\0';

    return line;
}

int main()
{
    BigNumber *num1 = createBigNumber();
    BigNumber *num2 = createBigNumber();
    BigNumber *result = createBigNumber();
    char operation, *strNum1, *strNum2;

    // Leitura dinâmica das strings com tratamento de erros
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
        // Adicione outros casos conforme necessário para outras operações

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