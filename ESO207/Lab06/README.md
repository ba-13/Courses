# Lab Assignment 6

## Linear Selection

    (a) Implement linear time selection algorithm. And use it to find the median.
    (b) Compare running time for random integers with faster of Merge/Heap sort for `N = 10, 100, 1000, 10000, 100000, 1000000`

Please stop running a program as soon as running time for that program exceeds 3 minutes.

### Output

```bash
➜  Lab6 ./bin/selection ./_random_numbers.txt 1000000  
Time taken by select(): 10.430000ms
0.500000 order static: 50047
Time taken by quickSort(): 83.889000ms
0.500000 order static: 50047
```

## Strassen's Algorithm

    (a) Implement Strassen's Algorithm to multiply two  $$2^k\mult2^k$$ matrices.
    (b) Compare running time with the usual method for multiplying two matrices (using random integers in range 0 to 99 (inclusive)) for `k = 4, 7, 9, 11, 13, 15, 17`

Please stop running a program as soon as running time for that program exceeds 3 minutes.

Write CPU time taken in each case for both problems in a Table (on paper), and handover the paper to TA (after writing your name/roll no). In case, you wish to upload your assignments, please upload this table also as a txt/csv file.

### Output

Trivial Multiplication

```bash
➜  Lab6 ./bin/strassen ./_random_numbers.txt 6 0 
Time taken by multiply(): 0.001000ms
➜  Lab6 ./bin/strassen ./_random_numbers.txt 12 0
Time taken by multiply(): 0.010000ms
➜  Lab6 ./bin/strassen ./_random_numbers.txt 25 0
Time taken by multiply(): 0.110000ms
➜  Lab6 ./bin/strassen ./_random_numbers.txt 50 0
Time taken by multiply(): 0.744000ms
➜  Lab6 ./bin/multiply ./_random_numbers.txt 512 0
Time taken: 393.145000ms
```

Strassen's Algorithm (what the hell was this)

```bash
➜  Lab6 ./bin/strassen ./_random_numbers.txt 512 0 
Time taken: 13228.821000ms
```
