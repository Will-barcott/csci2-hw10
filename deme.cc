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
  for (unsigned i; i <= pop_size; i++){
    pop_.push_back(new Chromosome(cities_ptr));
  }
  mut_rate_ = mut_rate;
}

// Clean up as necessary
Deme::~Deme()
{
  for (auto i: pop_) {
    delete i;   //Deallocate memory from each element pointed to in pop_
  }
  pop_.clear(); //Erases elements in pop_
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

//Functor comparison object for best fitness.
struct compare_fitness {
  bool operator() (const Chromosome* p1, const Chromosome* p2) {
    return p1->get_fitness() < p2->get_fitness();
  }
}fit_cmp;

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  return *std::max_element(pop_.cbegin(), pop_.cend(), fit_cmp);
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
  //Get your total fitness value
  double totalFitness = std::accumulate(pop_, pop_.back());
  //create a vector to contain the probabilities of each result
  std::vector<std::pair<double, double>> probVector;
  /*Probability values will be stored as pairs representing cumulativeranges (a chromosome with a value of 99 and 
  a chromosome with a value of 1 will be represented as (0,0.99) and (0.99,1)) Not sure if this is the best way to 
  do it, but this should let us generate a random number then check what range its in to pick the chromosome
  Feel free to change if you have a better way to do this */
  double currentProb = 0.0;
  for (auto i : pop_){
    double fitness = i->get_fitness();
    double fitnessProb = fitness/totalFitness;
    double cumulativeFitnessProb = currentProb + fitnessProb;
    probVector.push_back(std::pair<double, double>(currentProb, cumulativeFitnessProb));
    currentProb += fitnessProb;
  }
  //generate random number from 0 to 1 - not the RNG we've been using, should change later
  std::srand(843190857);
  double rand = rand();
  //find the range in your vector that includes that number
  int indexSelected = 0;
  for (auto i : probVector){
    if ((std::get<0>(i) < rand) & (std::get<1>(i) > rand)){
      break;
    }
    indexSelected++;
  }
  //return the chromosome at that index
  Chromosome* selectedParent = pop_[indexSelected];
  return selectedParent;
}
 
