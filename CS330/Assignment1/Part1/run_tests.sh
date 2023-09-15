#!/bin/bash

test () {

    # Execute the given ops pattern.
    RESULT=`echo "$($2)"`
    # If produced result and expected result is equal then test is passed.
    if [[ $RESULT -eq $3 ]] 
    then
        echo "TEST $1 PASSED"
    else
        echo "TEST $1 FAILED"
    fi

}

cd bin || echo "Didn't find bin directory"

# Cleanup old executable 
[ -f sqroot ] && rm sqroot
[ -f double ] && rm double
[ -f square ] && rm square

# Compile
gcc -o sqroot ../sqroot.c -lm
gcc -o double ../double.c -lm
gcc -o square ../square.c -lm

# Tests
test 1 "./sqroot 5" 2
test 2 "./double square 2" 16
test 3 "./sqroot square 4" 4
