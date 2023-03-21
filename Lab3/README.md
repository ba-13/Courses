# Lab Assignment 3

1. Write code to generate random integers (in suitable range). Your output should be in a file. You may use C-library routines or any method (see e.g. <https://en.wikipedia.org/wiki/List_of_random_number_generators>)

2. Implement insertion sort to sort integers. Your program must read numbers from a file (say that created in 1st problem). You may "hard-code" the file name or any other method you are familiar with (e.g. command line arguments). The number of items can again be either read from terminal/file/command line argument.

3. Implement heap sort to sort integers. Your program must read numbers from a file (say that created in 1st problem). You may "hard-code" the file name or any other method you are familiar with (e.g. command line arguments). The number of items can again be either read from terminal/file/command line argument.

4. Use time command to find running time for programs 2 and 3 for:

> N = 10, 100, 1000, 10000, 100000 (1 lakh), 1000000 (1 Million)

Please stop running a program as soon as running time exceeds 3 minutes.

## Solution

```bash
mkdir bin
gcc -O4 generate_random_numbers.c -o bin/generate_random_numbers
gcc -O4 insertion_sort.c -o ./bin/insertion_sort
gcc -O4 merge_sort.c -o ./bin/merge_sort
gcc -O4 heap_sort.c -o ./bin/heap_sort

./bin/generate_random_numbers 1000000 1 100000
time ./bin/insertion_sort ./_random_numbers.txt 100000
time ./bin/merge_sort ./_random_numbers.txt 1000000
time ./bin/heap_sort ./_random_numbers.txt 1000000
```

### Insertion Sort Performance

```bash
./bin/insertion_sort    ./_random_numbers.txt 100000  6.01s user 0.00s system 99% cpu 6.016 total
./bin/insertion_sort    ./_random_numbers.txt 1000000  597.98s user 0.08s system 99% cpu 9:58.86 total
```

### Merge Sort Performance

```bash
./bin/merge_sort        ./_random_numbers.txt 100000  0.03s user 0.00s system 99% cpu 0.029 total
./bin/merge_sort        ./_random_numbers.txt 1000000  0.29s user 0.01s system 93% cpu 0.323 total
```

### Heap Sort Performance

```bash
./bin/heap_sort         ./_random_numbers.txt 100000  0.03s user 0.00s system 99% cpu 0.033 total
./bin/heap_sort         ./_random_numbers.txt 1000000  0.34s user 0.01s system 91% cpu 0.381 total
```
