# CSCI2-hw10
#### Authors: Ethan McDonald, Will Barcott

## Usage:
```
$ make
$ make test
```
Valgrind test used:
```
$ valgrind --tool=memcheck ./tsp [file] [pop_size] [mutate_rate]
```

### Results:
To generate ```ga.tsv```:
```
$ ./tsp challenge.tsv 101 0.101 | tee ga.tsv
```
To generate ```randomized.tsv```:
```
$ ./tsp challenge.tsv | tee randomized.tsv
```
Finally, these two files were plotted with ```gnuplot``` and the resulting gif is included.

## Design Commentary:
Overall this was an exercise of how many standard library functions we could use.

### Chromosome:
We used the professor's solution code for ```cities.*, tsp.cc```. The interesting part of Chromosome is choosing random numbers in a given range
for ```recombine()``` and ```mutate()```. Here's what we thought would be clever, fill up a vector in increasing order from ```[0, n]```, then
shuffle the vector using ```std::shuffle```, and then pick up the first two values in the vector. The less interesting, but crucial element is the fitness function. We chose to implement a simple fitness function that takes the inverse of the path distance to reach our desired goal of inversely proportional fitnesses and path distances. This could be interesting to experiment with in the hopes of forcing increased elitism in gene selection.

### Deme:
I think the interesting function is ```select_parent()```. Here, we used roulette wheel selection. So, we first used ```std::accumulate``` to get the sum of each Chromosome's fitness. Then we created a table with ```pop_.size()``` indices, and each position in the table contained the normalized probability for the respective Chromosome. Then we generate a normalized probability, and iterate through our table until we find which Chromosome is the lucky winner!