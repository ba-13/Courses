# Lab Assignment 9

RB-Trees are binary search trees in which each node has a non-negative integral rank (rank is 0, 1, 2,...) such that

- Rank of failure nodes is 0
- Rank of parent of failure node is 1
- If "x" is any node and p(x) is parent of x, then rank(x) <= rank(p(x)) <= rank(x)+1
- If g(x) is p(p(x)), the grand parent of x, then rank(x) <= rank(g(x))-1

Implement following operations on RB-trees:

- Search
- Insert
- Delete

Try to insert 500 random numbers one by one. Then, repeatedly find, print and delete the minimum item (for 100 times)

You are free to reuse code of lab 8. You may have to use/modify/generalise these programs in future labs. Thus, please save your programs.

# Help

- [Balanced Trees](https://nscpolteksby.ac.id/ebook/files/Ebook/Computer%20Engineering/Algorithm%20Design%20and%20Applications%20A4%20(2015)/5.%20Chapter%204%20-%20Balanced%20Binary%20Search%20Trees.pdf)
- [Slides](https://www.ics.uci.edu/~goodrich/teach/cs165/notes/WeakAVLTrees.pdf)