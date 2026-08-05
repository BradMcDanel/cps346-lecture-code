## Build instructions

```
cd 01-why-parallel-computing/
mkdir build
cd build
cmake ..
make
```

## What this is

`count_primes` counts the prime numbers below 40,000,000 the slow and obvious
way. We used it in the first class as a demo. You are not expected to
understand the parallel version yet - that is what the rest of the semester is
for.

```
./count_primes            # uses the default limit
./count_primes 5000000    # or pass your own
```

The correct answer is `2433654`.
