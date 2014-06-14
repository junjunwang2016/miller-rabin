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

notice
------

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

miller-rabin 素数测试 (基于大整型)
----------------------------------

update: 2014.06.09 by xkuga
    
函数: GenPrime(bitLen, times, result)

可生成一个指定位长和miller-rabin测试次数的素数

生成一个1024bit的素数大约需要6小时

生成一个512bit的素数大约需要3小时

生成一个100bit的素数大约需要1分钟

生成一个32bit的素数大约需要4秒

在生成一个大素前，请确保宏定义 BIG_INT_BIT_LEN 足够大

注意
----

素数的生成不是随机的

他是首先检测指定位长中最大的一个奇数，也就是全1的情况

然后再每次减2，再测试，直到通过miller-rabin测试为止

因此程序总是生成指定整型位数中最大的一个素数

当然你也可以随机去生成素数，程序已经写了相关函数

不过随机生成的话只能祝你好运了，而且我们一般需要更大的素数

BigInt 大整型（程序已实现）
----------------------------

程序实现了这几种大数运算：+, -, *, /, ^, %, ^%

采用的是二进制的方式实现, BigInt的存储与运算均采用补码

乘法：采用的是Booth一位乘

除法：用二分法去做减法

幂运算与模幂运算：都是采用二进制的方式实现的，并非一个一个乘

具体实现原理可看微博@C程序设计语言：http://www.weibo.com/cpl86

虽然可能不是很快，不过还是能用><

而且程序比较易懂，对于理解计算机如何运算有一定的帮助

如果有任何错误或改进可在微博中@C程序设计语言，谢谢!

完整的代码，可到Github下载: https://github.com/xkuga/miller-rabin
