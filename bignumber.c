#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para duplicar uma string
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

// Função auxiliar para inverter uma string
static void reverseString(char *str)
{
    int length = strlen(str);
    int i, j;
    for (i = 0, j = length - 1; i < j; i++, j--)
    {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

// Função auxiliar para obter o valor numérico de um caractere
int charToDigit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else
    {
        fprintf(stderr, "Erro: tentativa de converter um caractere invalido em digito\n");
        exit(EXIT_FAILURE);
    }
}

// Função auxiliar para obter o caractere correspondente a um dígito
char digitToChar(int digit)
{
    if (digit >= 0 && digit <= 9)
    {
        return '0' + digit;
    }
    else
    {
        fprintf(stderr, "Erro: tentativa de converter um digito invalido em caractere\n");
        exit(EXIT_FAILURE);
    }
}

// Implementação de createBigNumber
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

// Implementação de destroyBigNumber
void destroyBigNumber(BigNumber *num)
{
    free(num->digits);
    free(num);
}

// Implementação de setBigNumberFromString
void setBigNumberFromString(BigNumber *num, const char *str)
{
    free(num->digits); // Libera a memória existente, se houver
    num->digits = my_strdup(str);
}

// Implementação de getBigNumberAsString
char *getBigNumberAsString(const BigNumber *num)
{
    return my_strdup(num->digits);
}

// Implementação de addBigNumbers
void addBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
    const char *str1 = num1->digits;
    const char *str2 = num2->digits;

    int sign1 = 1;
    int sign2 = 1;

    // Remove o sinal dos números
    if (str1[0] == '-')
    {
        sign1 = -1;
        str1++;
    }

    if (str2[0] == '-')
    {
        sign2 = -1;
        str2++;
    }

    reverseString(my_strdup(str1)); // Alteração 1
    reverseString(my_strdup(str2)); // Alteração 1

    int len1 = strlen(str1), len2 = strlen(str2);

    int maxSize = (len1 > len2) ? len1 : len2;
    maxSize++; // Tamanho máximo do resultado é o máximo entre len1 e len2, mais 1 para o possível carry

    // Realoca a memória do resultado conforme necessário
    result->digits = (char *)realloc(result->digits, (maxSize + 2) * sizeof(char)); // Alteração 2
    if (!result->digits)
    {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(EXIT_FAILURE);
    }

    int carry = 0;
    int i, sum;

    for (i = 0; i < maxSize || carry; i++)
    {
        int digit1 = (i < len1) ? charToDigit(str1[i]) : 0;
        int digit2 = (i < len2) ? charToDigit(str2[i]) : 0;

        sum = (sign1 * digit1) + (sign2 * digit2) + carry;

        if (sum < 0)
        {
            sum += 10;
            carry = -1;
        }
        else
        {
            carry = sum / 10;
        }

        result->digits[i] = digitToChar(sum % 10);
    }

    result->digits[i] = '\0';

    reverseString(result->digits);

    // Adiciona o sinal ao resultado
    if ((sign1 == -1 && sign2 == -1) || (sign1 == 1 && sign2 == -1 && len2 > len1) ||
        (sign1 == -1 && sign2 == 1 && len1 > len2))
    {
        // Se ambos são negativos ou num1 é positivo e maior ou igual a num2
        result->digits = (char *)realloc(result->digits, (i + 2) * sizeof(char));
        if (!result->digits)
        {
            fprintf(stderr, "Erro na alocação de memória\n");
            exit(EXIT_FAILURE);
        }

        memmove(result->digits + 1, result->digits, i + 1);
        result->digits[0] = '-';
    }
    else if (result->digits[0] == '0' && result->digits[1] != '\0' && result->digits[1] != '-')
    {
        // Remove o zero à esquerda, se o resultado não for zero
        memmove(result->digits, result->digits + 1, i);
        result->digits = (char *)realloc(result->digits, i * sizeof(char));
        if (!result->digits)
        {
            fprintf(stderr, "Erro na alocação de memória\n");
            exit(EXIT_FAILURE);
        }
    }
}