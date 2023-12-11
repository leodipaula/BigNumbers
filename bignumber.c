#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int isGreaterThan(const char *num1, const char *num2)
{
    while (*num1 == '0' && *(num1 + 1) != '\0')
    {
        num1++;
    }

    while (*num2 == '0' && *(num2 + 1) != '\0')
    {
        num2++;
    }

    int isNegative1 = (*num1 == '-');
    int isNegative2 = (*num2 == '-');

    if (isNegative1 && !isNegative2)
        return 0;

    if (!isNegative1 && isNegative2)
        return 1;

    if (isNegative1 && isNegative2)
        return strcmp(num2, num1) > 0;

    int len1 = strlen(num1);
    int len2 = strlen(num2);

    if (len1 > len2)
        return 1;

    if (len1 < len2)
        return 0;

    return strcmp(num1, num2) > 0;
}

static char *my_strdup(const char *str)
{
    size_t len = strlen(str) + 1;
    char *new_str = (char *)malloc(len);

    if (new_str == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria em my_strdup\n");
        exit(EXIT_FAILURE);
    }

    return strcpy(new_str, str);
}

void reverseString(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int charToDigit(char c)
{
    return (c >= '0' && c <= '9') ? c - '0' : 0;
}

char digitToChar(int digit)
{
    if (digit >= 0 && digit <= 9)
        return '0' + digit;
    else
    {
        fprintf(stderr, "Erro: tentativa de converter um digito invalido em caractere\n");
        fprintf(stderr, "Valor do digito: %d\n", digit);
        exit(EXIT_FAILURE);
    }
}

BigNumber *createBigNumber()
{
    BigNumber *num = (BigNumber *)malloc(sizeof(BigNumber));
    if (num == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para BigNumber\n");
        exit(EXIT_FAILURE);
    }
    num->digits = NULL;
    return num;
}

void destroyBigNumber(BigNumber *num)
{
    free(num->digits);
    free(num);
}

void setBigNumberFromString(BigNumber *num, const char *str)
{
    free(num->digits);
    int len = strlen(str);
    num->digits = (char *)malloc(len + 1);
    if (num->digits == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para os digitos do BigNumber\n");
        exit(EXIT_FAILURE);
    }
    strcpy(num->digits, str);
}

char *getBigNumberAsString(const BigNumber *num)
{
    return my_strdup(num->digits);
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

void subBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
    int len1 = strlen(num1->digits);
    int len2 = strlen(num2->digits);
    int len = len1 > len2 ? len1 : len2;
    char *res = (char *)malloc(len + 2); // +2 para possível carry e para '/0'
    if (res == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para resultado\n");
        exit(EXIT_FAILURE);
    }
    memset(res, '0', len + 1);
    res[len + 1] = '\0';

    int isNegative1 = num1->digits[0] == '-';
    int isNegative2 = num2->digits[0] == '-';

    int aux;
    if (abs(isGreaterThan(num1->digits, num2->digits)))
    {
        aux = 0;
    }
    else
    {
        aux = 1;
    }

    // Inverte os digitos para fazer a sub a mão
    reverseString(num1->digits);
    reverseString(num2->digits);

    int carry_sub = 0;
    int carry = 0;
    int digit1;
    int digit2;
    // Sub a mão
    for (int i = 0; i < len; i++)
    {
        digit1 = i < len1 ? abs(charToDigit(num1->digits[i])) : 0;
        digit2 = i < len2 ? abs(charToDigit(num2->digits[i])) : 0;
        int sum;
        if (isNegative1 && isNegative2)
        {
            if (aux == 1)
            {
                if (digit1 >= digit2)
                {
                    sum = (digit1 - carry_sub) - digit2;
                    carry_sub = 0;
                }
                else
                {
                    sum = (digit1 + 10 - carry_sub) - digit2;
                    carry_sub = 1;
                }
            }
            else
            {
                if (digit2 >= digit1)
                {
                    sum = (digit2 - carry_sub) - digit1;
                    carry_sub = 0;
                }
                else
                {
                    sum = (digit2 + 10 - carry_sub) - digit1;
                    carry_sub = 1;
                }
            }
        }
        else if (isNegative1)
        {
            sum = digit1 + digit2 + carry;
        }
        else if (isNegative2)
        {
            sum = digit1 + digit2 + carry;
        }
        else
        {
            if (aux == 0)
            {
                if (digit1 >= digit2)
                {
                    sum = (digit1 - carry_sub) - digit2;
                    carry_sub = 0;
                }
                else
                {
                    sum = (digit1 + 10 - carry_sub) - digit2;
                    carry_sub = 1;
                }
            }
            else
            {
                if (digit2 >= digit1)
                {
                    sum = (digit2 - carry_sub) - digit1;
                    carry_sub = 0;
                }
                else
                {
                    sum = (digit2 + 10 - carry_sub) - digit1;
                    carry_sub = 1;
                }
            }
        }
        carry = sum / 10;
        res[i] = digitToChar(sum);
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
    if (((isNegative1 && isNegative2)) && (isGreaterThan(num1->digits, num2->digits)))
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }
    else if (isNegative1 && !isNegative2)
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }
    if (((!isNegative1 && !isNegative2)) && (isGreaterThan(num2->digits, num1->digits)))
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }

    setBigNumberFromString(result, res);

    free(res);
    reverseString(num1->digits);
    reverseString(num2->digits);
}

void multiplyStrings(char *num1, char *num2, char *result)
{
    int len1 = strlen(num1);
    int len2 = strlen(num2);

    // Flag para verificar se o resultado deve ser negativo
    int isNegativeResult = 0;

    // Verifica se o primeiro número é negativo
    if (num1[0] == '-')
    {
        isNegativeResult = !isNegativeResult;
        memmove(num1, num1 + 1, len1); // Remove o sinal
        len1--;
    }

    // Verifica se o segundo número é negativo
    if (num2[0] == '-')
    {
        isNegativeResult = !isNegativeResult;
        memmove(num2, num2 + 1, len2); // Remove o sinal
        len2--;
    }

    // Reversing the strings
    reverseString(num1);
    reverseString(num2);

    int *m = calloc(len1 + len2, sizeof(int));

    for (int i = 0; i < len1; i++)
    {
        for (int j = 0; j < len2; j++)
        {
            m[i + j] += (num1[i] - '0') * (num2[j] - '0');
        }
    }

    for (int i = 0; i < len1 + len2; i++)
    {
        int digit = m[i] % 10;
        int carry = m[i] / 10;
        if (i + 1 < len1 + len2)
        {
            m[i + 1] += carry;
        }
        result[i] = digit + '0';
    }

    // Ignore leading zeros
    int i = len1 + len2 - 1;
    while (i > 0 && result[i] == '0')
    {
        i--;
    }
    result[i + 1] = '\0';

    // Reverse the result
    reverseString(result);

    // Adiciona o sinal negativo, se necessário
    if (isNegativeResult && result[0] != '0')
    {
        memmove(result + 1, result, strlen(result) + 1);
        result[0] = '-';
    }

    free(m);
}

void multiplyBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
    char *strNum1 = getBigNumberAsString(num1);
    char *strNum2 = getBigNumberAsString(num2);

    while (*strNum1 == '0' && *(strNum1 + 1) != '\0')
    {
        strNum1++;
    }
    while (*strNum2 == '0' && *(strNum2 + 1) != '\0')
    {
        strNum2++;
    }

    char *resultStr = (char *)malloc(MAX(strlen(strNum1), strlen(strNum2)) * 2 + 1);
    if (resultStr == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para resultado\n");
        exit(EXIT_FAILURE);
    }

    multiplyStrings(strNum1, strNum2, resultStr);

    setBigNumberFromString(result, resultStr);

    free(strNum1);
    free(strNum2);
    free(resultStr);
}