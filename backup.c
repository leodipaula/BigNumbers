void addBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result)
{
    int len1 = strlen(num1->digits);
    int len2 = strlen(num2->digits);
    int len = len1 > len2 ? len1 : len2;
    char *res = (char *)malloc(len + 2); // +2 for possible carry and null terminator
    if (res == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para resultado\n");
        exit(EXIT_FAILURE);
    }
    memset(res, '0', len + 1);
    res[len + 1] = '\0'; // Null terminate the string

    int isNegative1 = num1->digits[0] == '-';
    int isNegative2 = num2->digits[0] == '-';

    reverseString(num1->digits);
    reverseString(num2->digits);

    int carry = 0;
    int digit1;
    int digit2;
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
            sum = (digit1 < abs(digit2)) ? (digit2 - digit1 - carry) : (digit2 - digit1 - carry);
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
    // Remove leading zeros
    while (*res == '0' && *(res + 1) != '\0')
    {
        memmove(res, res + 1, strlen(res));
    }
    if (((isNegative1 && !isNegative2) && digit1 > digit2) ||
        ((isNegative2 && !isNegative1) && digit2 > digit1) ||
        (isNegative1 && isNegative2))
    {
        memmove(res + 1, res, strlen(res) + 1);
        res[0] = '-';
    }
    setBigNumberFromString(result, res);

    free(res);
    reverseString(num1->digits);
    reverseString(num2->digits);
}