#include <stdio.h>
#include "bignumber.h"
#include <stdlib.h>
#include <strings.h>

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

int main()
{
    BigNumber *num1 = createBigNumber();
    BigNumber *num2 = createBigNumber();
    BigNumber *result = createBigNumber();
    char operation, *strNum1, *strNum2;

    while (1)
    {
        strNum1 = readDynamicLine();
        if (strNum1 == NULL)
            break;
        strNum2 = readDynamicLine();
        if (strNum2 == NULL)
            break;
        operation = getchar(); // read the operation
        getchar();             // consume the newline

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
        fflush(stdout);

        free(strNum1);
        free(strNum2);
    }

    destroyBigNumber(num1);
    destroyBigNumber(num2);
    destroyBigNumber(result);

    return 0;
}

void addBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
    int len1 = strlen(num1->digits);
    int len2 = strlen(num2->digits);
    int len = len1 > len2 ? len1 : len2;
    char *res = (char *)malloc(len + 2);
    if (res == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para resultado\n");
        exit(EXIT_FAILURE);
    }
    memset(res, '0', len + 1);
    res[len + 1] = '\0';

    int isNegative1 = num1->digits[0] == '-';
    int isNegative2 = num2->digits[0] == '-';

    // Inverte os digitos para fazer a soma a mão
    reverseString(num1->digits);
    reverseString(num2->digits);

    int carry = 0;
    int digit1;
    int digit2;
    // Soma a mão
    for (int i = 0; i < len; i++)
    {
        digit1 = i < len1 ? abs(charToDigit(num1->digits[i])) : 0;
        digit2 = i < len2 ? abs(charToDigit(num2->digits[i])) : 0;
        int sum;
        if (isNegative1 && isNegative2)
        {
            sum = digit1 + digit2 + carry;
        }
        else if (isNegative1)
        {
            sum = (abs(digit1) > digit2) ? (digit1 - digit2 - carry) : (digit2 - digit1 - carry);
            if (sum < 0)
            {
                sum += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }
        }
        else if (isNegative2)
        {
            sum = (abs(digit2) > digit1) ? (digit2 - digit1 - carry) : (digit1 - digit2 - carry);
            if (sum < 0)
            {
                sum += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }
        }
        else
        {
            sum = digit1 + digit2 + carry;
        }
        res[i] = digitToChar(sum % 10);
        carry = sum / 10;
    }
    if (carry > 0)
    {
        res[len] = digitToChar(carry);
    }

    reverseString(res);
    // Remove zeros a esquerda
    while (*res == '0' && *(res + 1) != '\0')
    {
        memmove(res, res + 1, strlen(res));
    }

    // Adiciona sinal negativo caso necessário
    if ((isNegative1 && isNegative2))
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }
    else if (((isNegative1 && !isNegative2) && (isGreaterThan(num1->digits, num2->digits))) ||
             ((isNegative2 && !isNegative1) && (isGreaterThan(num2->digits, num1->digits))))
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }
    setBigNumberFromString(result, res);

    free(res);
    reverseString(num1->digits);
    reverseString(num2->digits);
}