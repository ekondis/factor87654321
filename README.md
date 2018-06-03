factor87654321
==============

This is a set of codes in various programming languages written in order to demonstrate the performance of a simple nested code loop. The problem selected is to find a pair of numbers in range [1,15000] the factor of which is equal to 87654321.

They are not supposed to be optimum codes neither the optimum algorithm for this purpose. The primary goal is to have a peek to the effectiveness of loops in some programming languages as well as to provide insight to the advantages of parallel programming paradigms (i.e. OpenMP & OpenCL).

Problem specification
--------------

The problem is defined as follows:
```
for i=1..15000
  for j=i+1..15000
    if i*j=87654321 then
      f_i = i
      f_j = j
    endif
```

Implementations
--------------

The provided implementations are written in the following programming languages:

1. Python
2. JavaScript
3. Java
4. Free pascal compiler
5. C
6. OpenMP/C
7. C + OpenCL

Example execution
---------------

Here are some results of the execution on an AMD Temash APU. More specifically the selected processor was the A6-1450 with 4 CPU cores and a GPU with 2 compute units (128 total shaders).

Execution | Execution time (msecs)
:---|-----:
Python (python 2.7.3, linux) | 42966.24  
Python (pypy JIT 1.8.0, linux) | 3302.54  
JavaScript (IE v11, Win8) | 918.00
JavaScript (Firefox v28, Win8) | 368.00
JavaScript (Chrome v34, Win8) | 782.00
Free Pascal Compiler (v.2.4.4, linux) | 710.00
C (gcc v.4.6.3, linux) | 293.64
C/OpenMP (gcc v.4.6.3, linux) | 129.53
OpenCL (CPU, AMD APP, linux) | 174.86
OpenCL (GPU, AMD APP, linux) | 38.61
