# Lab Assignment 5

1. Implement recursive quick sort, pivot is to be chosen at random.

2. Implement recursive merge sort. You are may use merge procedure (to merge two sorted arrays) of a previous lab.

3. Use time command to find running time for above programs and also non-recursive heap sort (of previous lab) for

> N = 10, 100, 1000, 10000, 100000, 1000000

Please stop running a program as soon as running time for that program exceeds 3 minutes.

## Quick Sort Perfomance

Realised that implementing the MicroOp is easy, and relying on SuperOp is the key, knowing how to divide the problem into these two is the tough part

### With pivot at end of list

#### With repeat for good pivot

```bash
➜  Lab5 ./bin/quick_sort ./_random_numbers.txt 1000000
83.181000 ms.
Bad Pivots chosen: 6964
Good Pivots chosen: 893122
Chance of getting a bad pivot: 0.007737
```

#### With no repeat for good pivot

```bash
➜  Lab5 ./bin/quick_sort ./_random_numbers.txt 1000000
111.410000 ms.
Bad Pivots chosen: 5677
Good Pivots chosen: 904853
Chance of getting a bad pivot: 0.006235
```

### With pivot at random

#### With repeat for good pivot

```bash
➜  Lab5 ./bin/quick_sort ./_random_numbers.txt 1000000
100.125000 ms.
Bad Pivots chosen: 7042
Good Pivots chosen: 893052
Chance of getting a bad pivot: 0.007824
```

#### With no repeat for good pivot

```bash
➜  Lab5 ./bin/quick_sort ./_random_numbers.txt 1000000
103.185000 ms.
Bad Pivots chosen: 7108
Good Pivots chosen: 899657
Chance of getting a bad pivot: 0.007839
```

## Merge Sort Performance

### No optimisation

```bash
➜  Lab5 ./bin/merge_sort ./_random_numbers.txt 1000000
120.908000 ms.
Number of merges performed: 999999
```

### Handling #arr = 2 without merge

```bash
➜  Lab5 ./bin/merge_sort ./_random_numbers.txt 1000000
111.424000 ms.
Number of merges performed: 524287
```
