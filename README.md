# How to compile the program

```bash
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

# How to run the program

```
MKP [problem instance] --[constructive heuristic] --[local search algorithm] [--verbose] [--seed]
```

[problem instance] is required and is the path to the problem instance to solve.

[constructive heuristic] is required and must be one of
- `--random`: Random solution construction.
- `--greedy`: Greedy solution construction.
- `--toyoda`: Toyoda algorithm.

[local search algorithm] is optional and must be one of
- `--FI`: First improvement local search.
- `--BI`: Best improvement local search.
- `--VND`: Variable neighborhood descent.

`--verbose` is optional and will print the full problem and solution to stdout, otherwise only the solution value is printed.
`--seed` is optional and will set the seed for the random number generator.

# Measurements.txt

This file contains the measurements of the recorded solution values and CPU times over all problem instances and algorithms.
The CPU time is reported in milliseconds. The headers of the columns are abbreviated as combinations of the following:
- `f`: FI
- `b`: BI
- `v`: VND


- `r`: random
- `g`: greedy
- `t`: toyoda


- `s`: solution value
- `t`: CPU time