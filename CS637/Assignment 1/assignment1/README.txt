1) Please install the LaTeX packages framed, float, titlesec, algorithms, stmaryrd, url, natbib, cascadilla, pgf before using this style file.

2) There are several macros defined in this style file. However, the ones that are most likely to be used have been demonstrated in the sample file itself.

3) To create a new question group, please use the environment escproblem

4) Individual question within a question groups are items in an enumeration

5) The command \answerbar produces a single bar for the student to write her solution

6) To obtain multiple bars, use the command \answerbars{n} where n is the number of bars desired

7) To get a blank space to allow the student to write a code by herself, use the command \newline\rectangle{width}{height}. A width of 18cm is appropriate for A4 size papers

8) To insert C code, use the listings package method (see Wikipedia/CTAN documentation). The package settings have been entered in the file escexam.sty and can be viewed/modified there.

9) Blank lines can be inserted into the C code by using the command %* \codeanswerbars{n} *) where n is the number of lines desired. The command \codeanswerbar produces a single such line.

10) The delimiters %* ... *) can be used to insert any LaTeX code into the C program

11) To get a uniform spacing, do not insert the LaTeX code %* \codeanswerbars{n} *) on a fresh line. Insert it into the last line with non-delimited C code. See the file prod.c for an example. The LaTeX code was inserted in line 5 itself and no fresh line was started to insert LaTeX code.

12) To produce a rectangular region with rule lines, use the command \ruledrectangle{n} where n is the number of lines desired.

13) Please go through sample.sty for header definition technique and other usage.