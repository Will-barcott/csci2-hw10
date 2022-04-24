/*
 * Implementation for Chromosome class
 */

#include "chromosome.hh"
#include <algorithm>
#include <cassert>
#include <chrono>

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(std::chrono::system_clock::now().time_since_epoch().count())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome.
// Fill an array (buffet) from 0, number of cities in order_.
// Shuffle buffet.
// Pick two items from the buffet as random indices into order_.
void
Chromosome::mutate()
{
  std::vector<int> buffet(order_.size(), 0);
  std::iota(buffet.begin(), buffet.end(), 0);
  std::shuffle(buffet.begin(), buffet.end(), generator_);

  int idx_one = buffet[0];
  int idx_two = buffet[1];

  auto first = order_.at(idx_one);
  order_.at(idx_one) = order_.at(idx_two);
  order_.at(idx_two) = first;

  assert(is_valid()); //Sanity check
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());

  std::vector<int> buffet(order_.size(), 0);
  std::iota(buffet.begin(), buffet.end(), 0);
  std::shuffle(buffet.begin(), buffet.end(), generator_);

  int idx_one = buffet[0];
  int idx_two = buffet[1];

  //Use these two indexes to crossover
  Chromosome* childOne = create_crossover_child(this, other, idx_one, idx_two);
  Chromosome* childTwo = create_crossover_child(other, this, idx_one, idx_two);

  return std::pair<Chromosome*, Chromosome*>(childOne, childTwo);
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
  return (1.0/pathDistance);
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
bool
Chromosome::is_valid() const
{
  std::vector<int> compare(order_.size(), 0);
  std::iota(compare.begin(), compare.end(), 0);    //Fill compare sequence.
  return std::is_permutation(compare.begin(), compare.end(), order_.begin());
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  for (unsigned i = begin; i < end; i++) {
    if (order_[i] == value) {
      return true;
    }
  }
  return false;
}
