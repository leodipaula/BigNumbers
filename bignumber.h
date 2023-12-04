#ifndef BIGNUMBER_H
#define BIGNUMBER_H

typedef struct
{
    char *digits;
} BigNumber;

BigNumber *createBigNumber();
void destroyBigNumber(BigNumber *num);
void setBigNumberFromString(BigNumber *num, const char *str);
char *getBigNumberAsString(const BigNumber *num);
void addBigNumbers(const BigNumber *num1, const BigNumber *num2, BigNumber *result);

#endif