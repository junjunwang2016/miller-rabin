miller-rabin
============

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
-------

the generate is not random

it first test the biggest BigInt, it means all bit is 1

then it subtract 2, and test again, unitl it find the prime

so it aways generate the biggest prime in the specify bit length

of course you can generate it randomly, good luck ><

BigInt
======

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
