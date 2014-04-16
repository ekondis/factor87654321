factor87654321
==============

This is a set of codes in various programming languages written in order to demonstrate the performance of a simple nested code loop. The problem selected is to find a pair of numbers in range [0,15000] which factor is equal to 87654321.

They are not supposed to be optimum codes neither the optimum algorithm for this purpose. The primary goal is to have a peek to the effectiveness of loops in some programming languages as well as to provide insight to the advantages of parallel programming paradigms.


Problem specification
--------------

The problem is defined as follows:
```
for i=0..15000
  for j=i+1..15000
    if i=j then
      f_i = i
      f_j = j
    endif
```

Implementations
--------------

The implementations are in the following programming languages:

1. Python (using python, pypy)
2. JavaScript (using IE, firefox, chrome)
3. Free pascal compiler
4. C (gcc)
5. OpenMP/C (gcc)
6. OpenCL

