# Assignment 2

Find problem statement [here](https://www.cse.iitk.ac.in/users/mainakc/2023Autumn/lec422/HW2.txt).

```bash
.
├── direction_predictor.hpp
├── instrument_dp.cpp
├── target_predictor.hpp
├── instrument_tp.cpp
├── makefile
├── makefile.rules
├── documentation.pdf
├── test_tp.cpp     # debugging target predictor
├── toy.cpp         # toy binary
└── HW2.md          # problem statement
```

## Execution

[Direction prediction](./instrument_dp.cpp) and [Target prediction](./instrument_tp.cpp) happen on different instrumentation files.

### Direction Prediction

A base class of `direction_predictor` is implemented that is derieved by all other types of predictors. Classes of `BHT` and `PHT` (of types `PHT_Norm` and `PHT_Xor` (used in gshare)) is implemented that is used as objects inside of table-based predictors.

All classes are then instantiated in `instrument_dp.cpp` using the base class pointer `direction_predictor`, making it convenient to add more classes and using the same code to run all of them.

### Target Prediction

A base class of `Cache` is implemented that implements LRU policy within a set via implementing a set using linked list.
A struct `Target` is a block saved in the Cache which we would want to retrieve. To implement the linked list out of this block, another struct `TargetHolder` is created.

Using the `Cache` instantiation, `BTB` and `BTB_pro` is implemented in nearly the same format, except:

- index of set is found using `ghr ^ pc` where `ghr` is being tracked by another instrumentation routine in `instrument_tp.cpp`
- tag of `pc` is the whole `pc`
