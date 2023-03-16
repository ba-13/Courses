# MIPS Help

## Review

<https://courses.cs.washington.edu/courses/cse378/09au/lectures/cse378au09-03.pdf>

### Syscalls

<https://courses.missouristate.edu/kenvollmar/mars/help/syscallhelp.html>

### Loading an address

<https://stackoverflow.com/questions/8380135/mips-assembly-lui-t0-4097>
Using lui for loading the upper 2 bytes, and ori with the lui-ed register for
ORing that with the lower 2 bytes, and storing in yet another final register

### Multiple MIPS Examples

<https://cs.ccsu.edu/~markov/ccsu_courses/254syllabus.html#CS%20254%20-%20Class%20#14>
<http://www0.cs.ucl.ac.uk/staff/electran/gc03/pdf/07mips_examples.pdf>

### Floating Point Instructions

<https://people.cs.pitt.edu/~sab104/teaching/cs447/labs/SlidesLab8.pdf>

### Working with Arrays

<https://courses.cs.vt.edu/~cs2505/summer2011/Notes/pdf/T23.MIPSArrays.pdf>

## Problem faced

Had been writing in space not allocated to the array, which replaced the next variable!
Freaking amazing, saw overflow, made overflow happen that is, not overflow tbh, just good variables being replaced by bad ones.

Am facing arithmetic and stack saving problems using $sp

## Conventions I follow

- Every comment at a line end signifies the comment for the whole block preceding it till the previous comment or the previous label
- function name (e.g. `bucket_sort`) is the label, and inner labels have the abbreviation of function appended as `_bs`
- A hierarchially upper label (e.g. `fill_buckets`) would also be appended to a lower label, as `_fb_bs`
