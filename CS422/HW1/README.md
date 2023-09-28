# CS422 Assignment 1

To analyze SPEC2006 Benchmark Applications using Intel PIN.
The applications under consideration are:

- 400.perlbench diffmail.pl (denoted as 400.perlbench)
- 401.bzip2 input.source (denoted as 401.bzip2)
- 403.gcc cp-decl.i (denoted as 403.gcc)
- 429.mcf
- 450.soplex ref.mps (denoted as 450.soplex)
- 456.hmmer nph3.hmm (denoted as 456.hmmer)
- 471.omnetpp
- 483.xalancbmk

Please find the exact problem statement [here](https://www.cse.iitk.ac.in/users/mainakc/2023Autumn/lec422/HW1.txt)

# Results

Please find the [PDF File](./documentation.pdf)

Also please find the output of all instrumentation routines per application inside [output](./output/)

# File Structure

```bash
.
├── commandline.txt     # commands to run applications 
├── common.h            # used by partC.cpp and partD.cpp
├── docs                # contains latex files used to generate the PDF
├── makefile
├── makefile.rules
├── output              # output/results from instrumentation
├── partA.cpp
├── partC.cpp
├── partD.cpp
├── README.md
├── total.cpp           # combination of partA, partC and partD
└── toy.cpp             # toy code for sanity check
```

`partA.cpp`, `partC.cpp` and `partD.cpp` contain instrumentation routines for those parts of the problem statement.
To build `total.cpp`, transfer this folder inside your `pin/source/tools/` directory.
You can download compatible PIN from [here](https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.28-98749-g6643ecee5-gcc-linux.tar.gz)

```bash
make obj-intel64/total.so
make TARGET=ia32 obj-ia32/total.so
```

Check functionality using:

```bash
g++ toy.cpp -o toy.out
pin -t obj-intel64/total.so -o toy.total.so -- ./toy.out
```

Get the set of applications to benchmark from [here](https://www.cse.iitk.ac.in/users/mainakc/2023Autumn/spec_2006.zip)