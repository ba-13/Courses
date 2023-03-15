# Lab 2 Results

## k = 16

### Mjolnir

- Slices - 48
- Flip Flops - 50
- LUTs - 73
- Delay - 5.329 ns

### Ripple

- Slices - 48
- Flip Flops - 50
- LUTS - 74
- Delay - 4.985 ns

## k = 32

### Mjolnir

- Slices - 117
- Flip Flops - 98
- LUTs - 152
- Delay - 7.388 ns

### Ripple

- Slices - 121
- Flip Flops - 98
- LUTS - 157
- Delay - 6.611 ns

## k = 64

### Ripple

- Slices - 245
- Flip Flops - 194 // 232 max
- LUTS - 329
- Delay - 8.591 ns

### Mjolnir

- Slices - 279
- Flip Flops - 194
- LUTs - 382
- Delay - 9.036 ns

> Comment when k-bit “Mjolnir” will be more efficient than k-bit ripple carry adder.

For high values of k, the mjolnir design would be better, as it emulates the divide and conquer paradigm.
It's better when the delay of k-bit adder is greater than the delay due to k/2-bit adder and the selection MUX.
