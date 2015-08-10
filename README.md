# Miller-Rabin

Miller-Rabin test base on [BigInt](https://github.com/xkuga/bigint)

# Performance

* 1024 bit: about 6 hours
* 512 bit: about 3 hours
* 100 bit: about 1 minute
* 32 bit: about 4 seconds

# Usage

bitlen: bit length, times: times of Miller-Rabin test

    GenPrime(bitLen, times, result)

# Notice

The generation is not random.
It first test the biggest BigInt, namely all bit is 1.
Then it subtract 2 and test again unitl it find the prime.
So it aways generate the biggest prime.

# BigInt

[https://github.com/xkuga/bigint](https://github.com/xkuga/bigint)
