/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
  // Add your implementation here
  for (unsigned i; i <= pop-size, i++){
    pop_.push_back(new Chromosome(cities_ptr)));
  }
  mut_rate_ = mut_rate;
}

// Clean up as necessary
Deme::~Deme()
{
  // Add your implementation here
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  // Add your implementation here
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  // Add your implementation here
  Chromosome* bestChromosome;
  bool firstFlag = True;
  for (Chromosome* i : pop_){
    if (firstFlag == true){
      bestChromosome = i;
      firstFlag = false;
    }
    else{
      if (i->get_fitness() > bestChromosome->get_fitness()){
        bestChromosome = i;
      } //close if
    } //close else
  } //close for
  return bestChromosome;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
  //Get your total fitness value
  double totalFitness = std::accumulate(pop_, pop_.back());
  //create a vector to contain the probabilities of each result
  std::vector<std::pair<double, double> probVector;
  /*Probability values will be stored as pairs representing cumulativeranges (a chromosome with a value of 99 and 
  a chromosome with a value of 1 will be represented as (0,0.99) and (0.99,1)) Not sure if this is the best way to 
  do it, but this should let us generate a random number then check what range its in to pick the chromosome
  Feel free to change if you have a better way to do this */
  double currentProb = 0.0;
  for (auto i : pop_){
    double fitness = i->get_fitness;
    double fitnessProb = fitness/totalFitness
    double cumulativeFitnessProb = currentProb + fitnessProb;
    probVector.push_back(std::pair<double, double>(currentProb, cumulativeFitnessProb));
    currentProb += fitnessProb;
  }
  //generate random number from 0 to 1
  //find the range in your vector that includes that number
  //return the chromosome at that index
  
}
