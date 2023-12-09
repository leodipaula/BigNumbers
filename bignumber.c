#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função que compara se o primeiro número é maior que o segundo
int isGreaterThan(const char *num1, const char *num2)
{
    // Remove possíveis zeros à esquerda
    while (*num1 == '0' && *(num1 + 1) != '\0')
    {
        num1++;
    }

    while (*num2 == '0' && *(num2 + 1) != '\0')
    {
        num2++;
    }

    // Verifica se os números são negativos
    int isNegative1 = (*num1 == '-');
    int isNegative2 = (*num2 == '-');

    // Se apenas um é negativo, o negativo é considerado menor
    if (isNegative1 && !isNegative2)
    {
        return 0;
    }
    else if (!isNegative1 && isNegative2)
    {
        return 1;
    }

    // Se ambos são negativos, inverte a comparação
    if (isNegative1 && isNegative2)
    {
        return strcmp(num2, num1) > 0;
    }

    // Obtemos os tamanhos finais após a remoção dos zeros à esquerda
    int len1 = strlen(num1);
    int len2 = strlen(num2);

    if (len1 > len2)
    {
        return 1;
    }
    else if (len1 < len2)
    {
        return 0;
    }

    // Se os tamanhos são iguais, compara numericamente
    return strcmp(num1, num2) > 0;
}

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

// Função auxiliar para obter o valor numérico de um caractere
int charToDigit(char c)
{
    return (c >= '0' && c <= '9') ? c - '0' : 0;
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
        fprintf(stderr, "Valor do digito: %d\n", digit);
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
    // Remove  zeros a esquerda
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
            if(aux == 1)
            {
                if(digit1 >= digit2)
                {
                    sum = (digit1-carry_sub) - digit2;
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
                if(digit2 >= digit1)
                {
                    sum = (digit2-carry_sub) - digit1;
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
            if(aux == 0)
            {
                if(digit1 >= digit2)
                {
                    sum = (digit1-carry_sub) - digit2;
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
                if(digit2 >= digit1)
                {
                    sum = (digit2-carry_sub) - digit1;
                    carry_sub = 0;
                }
                else
                {
                    sum = (digit2 + 10 - carry_sub) - digit1;
                    carry_sub=1;
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


//implementação da multiplicação com algoritmo de karatsuba
int karatsuba (const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
  // Determina o tamanho do menor numero
  int n = strlen(num1->digits);
  if (strlen(num2->digits) > n) n = strlen(num2->digits);

  // caso base: tamanho<=2, multiplica direto 
  if (n <= 2) {
    long long int aux = atoll(num1->digits) * atoll(num2->digits); // Converte string para long long int
    char* r = (char*)malloc(sizeof(char) * 21); // Aloca memoria para resultado
    sprintf(r, "%lld", aux); // Converte long long int para string
    return r;
  }

  // completa strings com zeros para ter mesmo tamanho n
  num1 = (char*)realloc(num1->digits, sizeof(char) * (n + 1));
  num2 = (char*)realloc(num2->digits, sizeof(char) * (n + 1));
  for (int i = n - 1; i >= 0; i--) {
    num1[i + 1] = num1[i]; // coloca caracteres a direita
  }
  num1[0] = '0'; // Adiciona 0
  for (int i = n - 1; i >= 0; i--) {
    num2[i + 1] = num2[i]; // coloca caracteres a direita
  }
  num2[0] = '0'; // Adiciona 0

  // Calculo para subproblemas
  int shift1 = n / 2 + 1;
  int shift2 = n / 2;
  if (n % 2 != 0) {
    shift1++; // Incrementa espacos vazios
    shift2++;
  }

  // extrai sub-strings para cada parte da multiplicação
  char* num1_1 = strndup(num1, n / 2); // num1_1 é a primeira metade de num1
  char* num1_2 = strndup(num1 + n / 2, n / 2); // num1_2 é a segunda metade de num1
  char* num2_1 = strndup(num2, n / 2); // num2_1 é a primeira metade de num2
  char* num2_2 = strndup(num2 + n / 2, n / 2); // num2_2 é a segunda metade de num2

  // função recursiva
  char* num1_1num2_1 = karatsuba(num1_1, num2_1, 0); // num1_1 * num2_1
  char* num1_2num2_2 = karatsuba(num1_2, num2_2, 0); // num2_2 * num2_2

  // Soma subproblemas
  char* sumAux = addBigNumbers(num1_1num2_1, num1_2num2_2, 0); // num1_1num2_1 + num1_2num2_2

  // Perform subtraction and shifting for the middle term
  char* num1_1num2_2 = subtractBigNumbers(karatsuba(addBigNumbers(num1_1, num1_2, 0), addBigNumbers(num2_1, num2_2, 0)), sumAux);
  shift(num1_1num2_2, shift2); // shift a1b2 de shift2

  // soma final
  char* r = addBigNumbers(addBigNumbers(shift(num1_1num2_1, shift1), num1_1num2_2), num1_2num2_2, 0); // soma de todas as partes

  // libera alocação de memória
  free(num1_1);
  free(num1_2);
  free(num2_1);
  free(num2_2);
  free(num1_1num2_1);
  free(num1_2num2_2);
  free(sumAux);
  return r;
}