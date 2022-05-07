# How to compile the program

```bash
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

# How to run the program

```
MKP [problem instance] --[constructive heuristic] --[local search algorithm] [--verbose] [--seed]
```
OR
```
MKP [problem instance] --[stochastic local search algorithm] [--verbose] [--seed]
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

[stochastic local search algorithm] is required and must be one of
- `--SA`: Simulate annealing algorithm.
- `--MA`: Memetic algorithm.

`--verbose` is optional and will print the full problem and solution to stdout, otherwise only the solution value is printed.
`--seed` is optional and will set the seed for the random number generator.

# data directory

 The data directory contains the measurements of solution quality performed for the second implementation exercise.
 It contains the following files:
 
- `data/MA.data`: Solution values for the memetic algorithm.
- `data/SA.data`: Solution values for the simulated annealing algorithm.
- `data/MA_1.data`: Solution values for the memetic algorithm for the first 5 instances of size 250 over 25 runs at max-time.
- `data/MA_10.data`: Solution values for the memetic algorithm for the first 5 instances of size 250 over 25 runs at max-time * 0.1.
- `data/MA_100.data`: Solution values for the memetic algorithm for the first 5 instances of size 250 over 25 runs at max-time * 0.01.
- `data/SA_1.data`: Solution values for the simulated annealing algorithm for the first 5 instances of size 250 over 25 runs at max-time.
- `data/SA_10.data`: Solution values for the simulated annealing algorithm for the first 5 instances of size 250 over 25 runs at max-time * 0.1.
- `data/SA_100.data`: Solution values for the simulated annealing algorithm for the first 5 instances of size 250 over 25 runs at max-time * 0.01.
