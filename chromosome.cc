/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>
#include <limits>
#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  // Add your implementation here
  assert(order_.size() > 1); //sanity check
  std::uniform_int_distribution<unsigned> distribution(0, order_.size()-1);
  unsigned indexOne = distribution(generator_);
  unsigned indexTwo = distribution(generator_);
  while (indexOne == indexTwo){
    indexTwo = distribution(generator_);
  }
  order_[indexOne], order_[indexTwo] = order_[indexTwo], order_[indexOne];
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  //Generate a random index of order_ from 0 to its second to last item
  std::uniform_int_distribution<unsigned> distribution(0, order_.size()-2);
  unsigned lowerIndex = distribution(generator_);
  //Generate a random index of order_ from that lower index to the maximum index
  std::uniform_int_distribution<unsigned> distributionTwo(lowerIndex+1, order_.size());
  unsigned higherIndex = distributionTwo(generator_);
  //Use these two indexes to crossover
  Chromosome* childOne = create_crossover_child(this, other, lowerIndex, higherIndex);
  Chromosome* childTwo = create_crossover_child(other, this, lowerIndex, higherIndex);
  //sanity check
  assert(is_valid());
  assert(other->is_valid());
  assert(childOne->is_valid());
  assert(childTwo->is_valid());
  std::pair<Chromosome*, Chromosome*> returnPair(childOne, childTwo);
  return returnPair;
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
        assert(j < p2->order_.size());
      }
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).

double
Chromosome::get_fitness() const
{
    double pathDistance = cities_ptr_->total_path_distance(order_);
    return (1/pathDistance);
  // Add your implementation here
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
bool
Chromosome::is_valid() const
{
  // Add your implementation here
  Cities::permutation_t newVector;
  for (unsigned int i = 0; i < order_.size(); i++){
      newVector.push_back(i);
  }
  return std::is_permutation(newVector.begin(), newVector.end(), order_.end());
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  unsigned int* beginPointer = &(begin);
  unsigned int* endPointer = &(end);
  int valueCount = std::count(beginPointer, endPointer, value);
  assert(valueCount < 2);
  if (valueCount == 0){
    return false;
  }
  else{
    return true;
  }
}
