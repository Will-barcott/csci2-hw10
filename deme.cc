/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "deme.hh"
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <cassert>
#include <iostream>

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
  : mut_rate_(mut_rate)
{
  for (unsigned i = 0; i < pop_size; i++){
    pop_.push_back(new Chromosome(cities_ptr));
  }
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  generator_ = std::default_random_engine(seed);
}

// Clean up as necessary
Deme::~Deme()
{
  for (auto i: pop_) {
    delete i;   //Deallocate memory from each element pointed to in pop_
  }
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select_parent() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  std::vector<std::pair<Chromosome*, Chromosome*>> chromosome_storage;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  for (std::size_t i = 0; i < pop_.size()/2; i++) {
    //Step 1: Select two parents.
    Chromosome* p1 = select_parent(); Chromosome* p2 = select_parent();
    //Step 2: Mutate or not.
    auto mutate1 = (distribution(generator_) < mut_rate_);
    if (mutate1) {
      p1->mutate();
    }
    auto mutate2 = (distribution(generator_) < mut_rate_);
    if (mutate2) {
      p2->mutate();
    }
    //Step 3: Recombine parents. Store children.
    chromosome_storage.push_back(p1->recombine(p2));
  }

  //Step 4: Replace parents.
  for (auto i : pop_) {
    delete i;
  }
  pop_.clear();
  for (auto i : chromosome_storage) {
    pop_.push_back(i.first);
    pop_.push_back(i.second);
  }
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
  //Dereference iterator of max element based on fitness.
  return *std::max_element(pop_.cbegin(), pop_.cend(), fit_cmp);
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
  int POP_SIZE = pop_.size();
  //Find total population fitness.
  auto sum_fitness = [](double sum, const Chromosome* c) { return sum + c->get_fitness(); };
  double totalFitness = std::accumulate(pop_.begin(), pop_.end(), 0.0, sum_fitness);

  //Create table of probabilities for each vector. Maintaining vector order is critical. Could also use a std::map...
  std::vector<double> prob_table(POP_SIZE, 0);
  //Calculate probability of selection for each Chromosome in pop_.
  prob_table[0] = pop_[0]->get_fitness() / totalFitness;
  for (int i = 1; i < POP_SIZE; i++) {
    prob_table[i] = prob_table[i-1] + (pop_[i]->get_fitness() / totalFitness);
  }

  std::uniform_real_distribution<double> distribution (0.0, totalFitness);
  //Generate random number from 0 - total fitness, and then normalize.
  double random_selection = distribution(generator_) / totalFitness;

  //Find the winning Chromosome based on rng.
  for (int i = 0; i < POP_SIZE; i++) {
    if (random_selection < prob_table[i]) {
      return pop_[i];
    }
  }
  return nullptr;
}
 
