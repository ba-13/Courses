# Lab Assignment 7

## Naive Approach

```bash
ribbon@csews49:(Lab7) $ ./bin/naive_closest_pair ./_random_numbers.txt 10
Time: 1us
Closest: (4412.5, -9992.0) (4777.4, -9226.6)  Distance: 718989.170000

ribbon@csews49:(Lab7) $ ./bin/naive_closest_pair ./_random_numbers.txt 100
Time: 18us
Closest: (6514.3, 9048.0) (6557.8, 8925.3)  Distance: 16947.540000

ribbon@csews49:(Lab7) $ ./bin/naive_closest_pair ./_random_numbers.txt 1000
Time: 1634us
Closest: (-9114.0, -182.1) (-9115.7, -171.4)  Distance: 117.380000

ribbon@csews49:(Lab7) $ ./bin/naive_closest_pair ./_random_numbers.txt 10000
Time: 92254us
Closest: (-168.8, 8248.3) (-169.0, 8249.4)  Distance: 1.250000

ribbon@csews49:(Lab7) $ ./bin/naive_closest_pair ./_random_numbers.txt 50000
Time: 2338745us
Closest: (-7591.6, -5345.5) (-7591.6, -5345.3)  Distance: 0.040000
```

## Divide and Conquer Approach

```bash
ribbon@csews49:(Lab7) $ ./bin/closest_pair ./_random_numbers.txt 10
Time: 7us
Closest: (4412.5, -9992.0) (4777.4, -9226.6)  Distance: 718989.170000

ribbon@csews49:(Lab7) $ ./bin/closest_pair ./_random_numbers.txt 100
Time: 71us
Closest: (6514.3, 9048.0) (6557.8, 8925.3)  Distance: 16947.540000

ribbon@csews49:(Lab7) $ ./bin/closest_pair ./_random_numbers.txt 1000
Time: 849us
Closest: (-9114.0, -182.1) (-9115.7, -171.4)  Distance: 117.380000

ribbon@csews49:(Lab7) $ ./bin/closest_pair ./_random_numbers.txt 10000
Time: 9478us
Closest: (-169.0, 8249.4) (-168.8, 8248.3)  Distance: 1.250000

ribbon@csews49:(Lab7) $ ./bin/closest_pair ./_random_numbers.txt 50000
Time: 37730us
Closest: (-7591.6, -5345.3) (-7591.6, -5345.5)  Distance: 0.040000
```
