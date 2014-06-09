/*
    miller-rabin test base on BigInt
    update: 2014.06.09 by xkuga
    
    function: GenPrime(bitLen, times, result)
    generate prime by specify bit length and miller-rabin test times
    generate a 1024bit prime may need about 6 hours
    generate a 512bit prime may need about 3 hours
    generate a 100bit prime may need about 1 minute
    generate a 32bit prime may need about 4 seconds
    if you generate a big prime, make sure the BIG_INT_BIT_LEN is enough

    notice:
    the generate is not random
    it first test the biggest BigInt, it means all bit is 1
    then it subtract 2, and test again, unitl it find the prime
    so it aways generate the biggest prime in the specify bit length
    of course you can generate it randomly, good luck ><

    BigInt operation

    support operation: +, -, *, /, ^, %, ^%
    the program is implement by binary
    the BigInt use complement to store and calculate
    multiplication: implement by using booth algorithm
    division: implement by using binary search
    pow and powmod: implement by using binary way

    maybe the program is not very fast, but it works anyway.
    what's more, it's easy to learn and understand how the computer works
    the full program can download on Github: https://github.com/xkuga/miller-rabin
    if you find any mistakes, you can contact me on weibo(http://weibo.com/cpl86)
    thank you
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BIG_INT_BIT_LEN 2400           // bit int bit length
#define SIGN_BIT BIG_INT_BIT_LEN - 1   // index of sign bit
#define BUFFER_SIZE BIG_INT_BIT_LEN    // buffer size
#define POSITIVE 0                     // 0 for positive number
#define NEGATIVE 1                     // 1 for negative number

typedef struct    // type:BigInt, use complement to store
{
    char bit[BIG_INT_BIT_LEN];
}BigInt;

typedef struct    // type:Number, use true value to store
{
    char value[BIG_INT_BIT_LEN];  // value array
    int len;                      // array length
    int sign;                     // POSITIVE(0) or NEGATIVE(1)
}Number;

// print BigInt
void PrintBigInt(BigInt* a)
{
    int i;
    for (i = SIGN_BIT; i >= 0; i--)
        printf("%d", a->bit[i]);
    printf("\n");
}

// print Number
void PrintNumber(Number* n)
{
    int i;

    if (n->sign == NEGATIVE)
        printf("-");

    for (i = n->len - 1; i >= 0; i--)
    {
        if (n->value[i] > 9)  // if radix > 10
            printf("%c", n->value[i] - 10 + 'a');
        else
            printf("%d", n->value[i]);
    }
    printf("\n");
}

// change type: string to Number, return: Number*
Number* StrToNumber(char* str, Number* n)
{
    int i, j;

    if (str[0] == '-' || str[0] == '+')  // if str[0] is sign
    {
        n->len = strlen(str) - 1;
        n->sign = str[0] == '+' ? POSITIVE : NEGATIVE;

        for (i = 0, j = n->len; j > 0; j--, i++)
        {
            if (str[j] > '9')  // if radix > 10
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }
    else  // default sign is POSITIVE
    {
        n->len = strlen(str);
        n->sign = POSITIVE;

        for (i = 0, j = n->len - 1; j >= 0; j--, i++)
        {
            if (str[j] > '9')  // if radix > 10
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }

    return n;
}

// change type: Number to string, return char*
char* NumberToStr(Number* n, char* str)
{
    int i = 0, j;

    if (n->sign == NEGATIVE)
        str[i++] = '-';

    for (j = n->len - 1; j >= 0; j--)
    {
        if (n->value[j] > 9)  // if radix > 10
            str[i++] = n->value[j] - 10 + 'a';
        else
            str[i++] = n->value[j] + '0';
    }

    str[i] = '\0';

    return str;
}

// binary string to hexadecimal string
char* BinStrToHexStr(char* binStr, char* hexStr)
{
    int i, j, t;
    Number binNum;
    Number hexNum;

    StrToNumber(binStr, &binNum);

    hexNum.sign = binNum.sign;
    hexNum.len = (int)ceil(binNum.len / 4.0);

    for (i = 0; i < hexNum.len; i++)
    {
        j = 4 * i;

        t = binNum.value[j];

        if (j + 1 < binNum.len)
            t += 2 * binNum.value[j + 1];

        if (j + 2 < binNum.len)
            t += 4 * binNum.value[j + 2];

        if (j + 3 < binNum.len)
            t += 8 * binNum.value[j + 3];

        hexNum.value[i] = t;
    }

    return NumberToStr(&hexNum, hexStr);
}

// change string radix from srcRadix to dstRadix
char* ChangeStringRadix(char* str, int srcRadix, int dstRadix, char* resultStr)
{
    if (srcRadix < dstRadix)
    {
        char hexStr[BUFFER_SIZE];

        ChangeStringRadix(str, srcRadix, 2, resultStr);  // srcRadix to radix 2
        BinStrToHexStr(resultStr, hexStr);               // radix 2 to 16

        // radix 16 to dstRadix
        return ChangeStringRadix(hexStr, 16, dstRadix, resultStr);
    }

    if (srcRadix == dstRadix)
    {
        return strcpy(resultStr, str);
    }

    else  // srcRadix > dstRadix
    {
        int i, t;
        Number dividend;
        Number quotient;
        Number resultNum;

        // string to Number
        StrToNumber(str, &dividend);

        resultNum.len = 0;
        resultNum.sign = dividend.sign;

        while (dividend.len > 0)
        {
            quotient.len = dividend.len;

            // simulate the way we do division
            // when the cycle is end, t is the remainder
            for (t = 0, i = dividend.len - 1; i >= 0; i--)
            {
                t = t * srcRadix + dividend.value[i];
                quotient.value[i] = t / dstRadix;
                t = t % dstRadix;
            }

            // save the remainder
            resultNum.value[resultNum.len++] = t;

            // filter the unnecessary 0 in quotient
            for (i = quotient.len - 1; i >= 0 && quotient.value[i] == 0; i--);

            // set the next dividend length
            dividend.len = i + 1;

            // let the quotient be the next divident
            for (i = 0; i < dividend.len; i++)
            {
                dividend.value[i] = quotient.value[i];
            }
        }

        return NumberToStr(&resultNum, resultStr);
    }
}

// true form <=> complement
BigInt* ToComplement(BigInt* src, BigInt* dst)
{
    int i;

    if (src->bit[SIGN_BIT] == NEGATIVE)  // NEGATIVE
    {
        dst->bit[SIGN_BIT] = 1;

        for (i = 0; i < SIGN_BIT && src->bit[i] == 0; i++)
            dst->bit[i] = src->bit[i];

        if (i == SIGN_BIT)  // notice: this is -0 complement
            dst->bit[i] = 0;
        else
        {
            dst->bit[i] = src->bit[i];
            for (i++; i < SIGN_BIT; i++)
                dst->bit[i] = !src->bit[i];
        }
    }
    else  // POSITIVE
    {
        for (i = 0; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = src->bit[i];
    }

    return dst;
}

// complement to true form
BigInt* ToTrueForm(BigInt* src, BigInt* dst)
{
    return ToComplement(src, dst);
}


// [x] complement to [-x] complement
// notice: for example, if BigInt bit length is 8
// it can not get the [-128] complement, overflow
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    int i;

    for (i = 0; i < BIG_INT_BIT_LEN && src->bit[i] == 0; i++)
        dst->bit[i] = src->bit[i];

    // if src is not 0
    if (i != BIG_INT_BIT_LEN)
    {
        dst->bit[i] = src->bit[i];

        // the sign bit should reverse too
        for (i++; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = !src->bit[i];
    }

    return dst;
}

/* basic implement
// [x] complement to [-x] complement
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    BigInt t;

    ToTrueForm(src, &t);
    t.bit[SIGN_BIT] = !t.bit[SIGN_BIT];
    ToComplement(&t, dst);

    return dst;
}
*/

// change type: binary Number to BigInt
BigInt* BinNumToBigInt(Number* binNum, BigInt* a)
{
    int i;

    memset(a->bit, 0, BIG_INT_BIT_LEN);  // init 0

    for (i = 0; i < binNum->len; i++)
    {
        a->bit[i] = binNum->value[i];
    }

    // if BigInt is the smallest nagative number
    // for example, if BigInt bit len is 4, BigInt is 1000
    if (binNum->len == BIG_INT_BIT_LEN)
    {
        return a;
    }
    else
    {
        a->bit[SIGN_BIT] = binNum->sign;
        return ToComplement(a, a);
    }
}

// change type: BigInt to binary Number
Number* BigIntToBinNum(BigInt* a, Number* binNum)
{
    int i;
    BigInt t;

    binNum->sign = a->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1; i >= 0 && a->bit[i] == 0; i--);

    // if BigInt is the smallest nagative number
    // for example, if BigInt bit len is 4, BigInt is 1000
    if (binNum->sign == NEGATIVE && i == -1)
    {
        binNum->len = BIG_INT_BIT_LEN;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = a->bit[i];
    }
    else
    {
        ToTrueForm(a, &t);
        for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);
        binNum->len = i == -1 ? 1 : i + 1;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = t.bit[i];
    }

    return binNum;
}

// change type: string to BigInt, use complement to store
BigInt* StrToBigInt(char* s, BigInt* a)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    ChangeStringRadix(s, 10, 2, buf);    // string radix 10 to 2
    StrToNumber(buf, &binNum);           // string to Number
    return BinNumToBigInt(&binNum, a);   // Number to BigInt
}

// change type: BigInt to string(radix 10)
char* BigIntToStr(BigInt* a, char* s)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    BigIntToBinNum(a, &binNum);     // BigInt to Number
    NumberToStr(&binNum, buf);      // Number to string

    return ChangeStringRadix(buf, 2, 10, s);  // string radix 2 to 10
}

// copy BigInt
BigInt* CopyBigInt(BigInt* src, BigInt* dst)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
        dst->bit[i] = src->bit[i];
    return dst;
}

// Shift Arithmetic Left
BigInt* ShiftArithmeticLeft(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

// Shift Arithmetic Right
BigInt* ShiftArithmeticRight(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = 0, j = i + indent; j < SIGN_BIT; i++, j++)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i < SIGN_BIT)
    {
        dst->bit[i++] = src->bit[SIGN_BIT];
    }

    return dst;
}

// implement of Addition
// result = a + b
BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result)
{
    int i, t, carryFlag;
    int aSign = a->bit[SIGN_BIT];  // a sign
    int bSign = b->bit[SIGN_BIT];  // b sign

    for (carryFlag = i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        t = a->bit[i] + b->bit[i] + carryFlag;
        result->bit[i] = t % 2;
        carryFlag = t > 1 ? 1 : 0;
    }

    if (aSign == bSign && aSign != result->bit[SIGN_BIT])
    {
        printf("Overflow XD\n");
        exit(1);
    }

    return result;
}

// implement of Subtraction
// result = a - b
BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result)
{
    BigInt t;

    ToOppositeNumberComplement(b, &t);
    DoAdd(a, &t, result);

    return result;
}

// implement of Multiplication by using Booth algorithm
// result = a * b
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result)
{
    int i;
    BigInt c, t;

    ToOppositeNumberComplement(a, &c);  // c = [-a] complement

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // init 0

    // filter
    for (i = SIGN_BIT; i > 0 && b->bit[i] == b->bit[i - 1]; i--);

    while (i > 0)
    {
        ShiftArithmeticLeft(&t, 1, &t);

        if (b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i - 1] > b->bit[i] ? a : &c, &t);
        }

        i--;
    }

    // the last shift
    ShiftArithmeticLeft(&t, 1, &t);
    if (b->bit[0] != 0)
    {
        DoAdd(&t, &c, &t);
    }

    return CopyBigInt(&t, result);
}

// get the max left shift length
int GetMaxLeftShiftLen(BigInt* a)
{
    int i, k;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1, k = 0; i >= 0 && t.bit[i] == 0; i--, k++);

    return k;
}

// get the max right shift length
int GetMaxRightShiftLen(BigInt* a)
{
    int i;

    for (i = 0; i < SIGN_BIT && a->bit[i] == 0; i++);

    return i;
}

// check if a bigint is 0
int IsZero(BigInt* a)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        if (a->bit[i] != 0)
            return 0;
    }
    return 1;
}

// implement of division by using binary search
// result = a / b
BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder)
{
    int low, high, mid;
    BigInt c, d, e, t;

    low = 0;                       // the min of left shift
    high = GetMaxLeftShiftLen(b);  // the max of left shift

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // init 0
    CopyBigInt(a, &c);                  // c = a

    // if a sign == b sign, do subtraction
    if (a->bit[SIGN_BIT] == b->bit[SIGN_BIT])
    {
        t.bit[SIGN_BIT] = POSITIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoSub(&c, &d, &e);  // e = c - d

                // e >= 0
                if (d.bit[SIGN_BIT] == e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high == -1 means c - b < 0
            if (high != -1)
            {
                t.bit[high] = 1;

                // here unified the operation
                // it can improve i think
                ShiftArithmeticLeft(b, high, &d);

                // c = c - d, let c be the next dividend
                DoSub(&c, &d, &c);

                low = 0;
                high--;
            }
            else
            {
                // now the dividend c is the remainder
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    // if a sign != b sign, do addition
    else
    {
        t.bit[SIGN_BIT] = NEGATIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoAdd(&c, &d, &e);  // e = c + d

                // e >= 0
                if (d.bit[SIGN_BIT] != e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high == -1 means c - b < 0
            if (high != -1)
            {
                t.bit[high] = 1;

                // here unified the operation
                // it can improve i think
                ShiftArithmeticLeft(b, high, &d);

                // c = c + d, let c be the next dividend
                DoAdd(&c, &d, &c);

                low = 0;
                high--;
            }
            else
            {
                // now the dividend c is the remainder
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    return ToComplement(&t, result);
}

// Addition
char* Add(char* s1, char* s2, char* result)
{
    BigInt a, b, c;
    
    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoAdd(&a, &b, &c);

    return BigIntToStr(&c, result);
}

// Subtraction
char* Sub(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoSub(&a, &b, &c);
    
    return BigIntToStr(&c, result);
}

// Multiplication
char* Mul(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMul(&a, &b, &c);

    return BigIntToStr(&c, result);
}

// Division
char* Div(char* s1, char* s2, char* result, char* remainder)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoDiv(&a, &b, &c, &d);
    BigIntToStr(&d, remainder);

    return BigIntToStr(&c, result);
}

// compare two BigInt
// a > b, return 1
// a = b, return 0
// a < b, resutn -1
int DoCompare(BigInt* a, BigInt* b)
{
    BigInt c;

    DoSub(a, b, &c);

    if (IsZero(&c))
        return 0;
    if (c.bit[SIGN_BIT] == POSITIVE)
        return 1;
    else
        return -1;
}

// compare two BigInt
// a > b, return 1
// a = b, return 0
// a < b, resutn -1
int Compare(char* s1, char* s2)
{
    BigInt a, b;

    StrToBigInt(s1, &a);
    StrToBigInt(s1, &b);

    return DoCompare(&a, &b);
}

// implement of mod, by using division
BigInt* DoMod(BigInt* a, BigInt* b, BigInt* remainder)
{
    BigInt c;

    DoDiv(a, b, &c, remainder);

    return remainder;
}

char* Mod(char* s1, char* s2, char* remainder)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMod(&a, &b, &c);

    return BigIntToStr(&c, remainder);
}

// get the length of true value
int GetTrueValueLen(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);

    return i + 1;
}

// implement of pow by using binary pow
BigInt* DoPow(BigInt* a, BigInt* b, BigInt* result)
{
    int i, len;
    BigInt t, buf;

    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
            DoMul(&t, &buf, &t);  // t = t * buf

        DoMul(&buf, &buf, &buf);  // buf = buf * buf
    }
    
    return CopyBigInt(&t, result);
}

char* Pow(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoPow(&a, &b, &c);

    return BigIntToStr(&c, result);
}

// implement of pow mod by using binary pow mod
BigInt* DoPowMod(BigInt* a, BigInt* b, BigInt* c, BigInt* result)
{
    int i, len;
    unsigned long t1, t2;
    BigInt t, buf;

    printf("    doing PowMod...\n");
    t1 = time(0);
    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
        {
            DoMul(&t, &buf, &t);  // t = t * buf
            DoMod(&t, c, &t);     // t = t % c;
        }

        DoMul(&buf, &buf, &buf);  // buf = buf * buf
        DoMod(&buf, c, &buf);     // buf = buf % c
    }
    t2 = time(0);
    printf("    finish PowMod (t=%lds)\n", t2 - t1);

    return CopyBigInt(&t, result);
}

char* PowMod(char* s1, char* s2, char* s3, char* result)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    StrToBigInt(s3, &c);
    DoPowMod(&a, &b, &c, &d);

    return BigIntToStr(&d, result);
}

// get odd random BigInt by specify the bit length
BigInt* DoGetOddRandBigInt(int bitLen, BigInt* result)
{
    int i;
    static unsigned long seed = 0;

    memset(result->bit, 0, BIG_INT_BIT_LEN);
    result->bit[0] = 1;

    srand(time(0) + seed++);
    for (i = 1; i < bitLen - 1; i++)
    {
        result->bit[i] = rand() % 2;
    }

    result->bit[i] = 1;

    return result;
}

char* GetOddRandBigInt(int bitLen, char* result)
{
    BigInt a;

    DoGetOddRandBigInt(bitLen, &a);

    return BigIntToStr(&a, result);
}

// get random BigInt from {1, 2, ..., n-1}
BigInt* DoGetRand(BigInt* n, BigInt* result)
{
    int i;
    static unsigned seed = 0;
    BigInt t;

    srand(time(0) + seed++);

    for (i = 0; i < SIGN_BIT; i++)
        t.bit[i] = rand() % 2;

    t.bit[SIGN_BIT] = 0;

    DoMod(&t, n, &t);
    if (IsZero(&t))
        DoGetRand(n, &t);

    return CopyBigInt(&t, result);
}

char* GetRand(char* s, char* result)
{
    BigInt a, b;

    StrToBigInt(s, &a);
    DoGetRand(&a, &b);

    return BigIntToStr(&b, result);
}

// miller rabin test
int DoMillerRabin(BigInt* n, int times)
{
    int i, j, s;
    BigInt a, t, x;
    BigInt one, two, nMinusOne;

    StrToBigInt("1", &one);      // one = 1
    StrToBigInt("2", &two);      // two = 2
    DoSub(n, &one, &nMinusOne);  // nMinusOne = n - 1

    // n-1 = 2^s * t
    s = GetMaxRightShiftLen(&nMinusOne);
    ShiftArithmeticRight(&nMinusOne, s, &t);

    for (i = 0; i < times; i++)
    {
        DoGetRand(n, &x);         // x = random{1, 2, ..., n-1}
        DoPowMod(&x, &t, n, &a);  // a = x^t % n, here is very slow><

        if (DoCompare(&a, &one) == 0)
            continue;

        for (j = 0; j < s; j++)
        {
            if (DoCompare(&a, &nMinusOne) == 0)
                goto LOOP;

            DoPowMod(&a, &two, n, &a);  // a = a^2 % n
        }

        return 0;

        LOOP:;
    }

    // return n is probably a prime
    return 1;
}

int MillerRabin(char* s, int times)
{
    BigInt n;

    StrToBigInt(s, &n);
    
    return DoMillerRabin(&n, times);
}

// generate prime by specify bit length and miller-rabin test times
// notice: the generate is not random
// it first test the biggest BigInt, it means all bit is 1
// then it subtract 2, and test again, unitl it find the prime
// so it aways generate the biggest prime in the specify bit length
// of course you can generate it randomly, good luck ><
BigInt* DoGenPrime(int bitLen, int times, BigInt* result)
{
    int i;
    unsigned long n = 1;
    unsigned long a, b;
    BigInt minusTwo;

    StrToBigInt("-2", &minusTwo);  // minusTwo = -2
    memset(result->bit, 0, BIG_INT_BIT_LEN);
    for (i = 0; i < bitLen; i++)   // set all 1, the biggest odd
        result->bit[i] = 1;

    while (1)
    {
        printf("testing number[%ld]...\n", n);
        a = time(0);

        if (DoMillerRabin(result, times))
        {
            b = time(0);
            break;
        }

        b = time(0);
        printf("finish test number %ld (t=%lds)\n\n", n++, b - a);

        DoAdd(result, &minusTwo, result);  // result = result - 2
    }

    printf("finish test number[%ld] (t=%lds)\n\n", n, b - a);

    return result;
}

char* GenPrime(int bitLen, int times, char* result)
{
    BigInt n;

    DoGenPrime(bitLen, times, &n);

    return BigIntToStr(&n, result);
}

int main()
{
    // printf("hello, world");

    unsigned long a, b;
    char result[BUFFER_SIZE];
    char remainder[BUFFER_SIZE];
    char buf[BUFFER_SIZE] = "prime=";

    // function: GenPrime(bitLen, times, result)
    // generate prime by specify bit length and miller-rabin test times
    // generate a 500bit prime may need about 3 hours
    // generate a 100bit prime may need about 1 minute
    // generate a 32bit prime may need about 4 seconds
    // if you generate a big prime, make sure the BIG_INT_BIT_LEN is enough

    a = time(0);
    puts(strcat(buf, GenPrime(100, 5, result)));
    b = time(0);
    printf("total t=%lds\n", b - a);
    
    /*
    // miller-rabin test
    a = time(0);
    printf("\n%s\n", MillerRabin("261", 5) ? "is prime" : "not prime");
    b = time(0);
    printf("total t=%lds\n", b - a);
    */
    
    // some primes
    // 4567, 124567, 3214567, 23456789, 55566677
    // 1234567894987654321, 11111111111111111111111(23)
    // 779711611410512054554699111109

    // the biggest prime in 100bit
    // 1267650600228229401496703205361

    // the biggest prime in 512bit t=13284s
    // 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006083527

    // the biggest prime in 1024bit t=20983s
    // 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137111

    // example: how to use BigInt
    //puts(Add("2010", "4", result));
    //puts(Sub("0", "2014", result));
    //puts(Mul("2", "43", result));
    //puts(Div("86", "10", result, remainder));
    //puts(remainder);
    //puts(Mod("-86", "10", result));
    //puts(PowMod("7", "80", "86", result));

    return 0;
}
