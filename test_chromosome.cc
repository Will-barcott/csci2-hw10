#include "chromosome.hh"
#include <sstream>
#include <iostream>

class TestChromosome : public Chromosome {
    public:
    TestChromosome(const Cities* cities_ptr)
        : Chromosome(cities_ptr)
        {}
    
    virtual ~TestChromosome() = default;

    void test_is_valid() {
        Cities::permutation_t testvec (10, 1);  //Vector of 10 ints == 1.
        order_.swap(testvec);
        assert(is_valid() == 0);

        std::iota(testvec.begin(), testvec.end(), 0);
        order_.swap(testvec);
        assert(is_valid() == 1);
    }

    void test_is_in_range() {
        Cities::permutation_t testvec(10,0);
        std::iota(testvec.begin(), testvec.end(), 0);
        order_.swap(testvec);

        assert(is_in_range(3, 0, 9) == 1);
        assert(is_in_range(1, 2, 9) == 0);
        assert(is_in_range(10, 0, 9) == 0);
        assert(is_in_range(5, 5, 6) == 1);
        assert(is_in_range(5, 4, 5) == 0);
    }

    void test_mutate() {
        Cities::permutation_t testvec = order_;
        mutate();

        assert(testvec != order_);
        assert(std::is_permutation(testvec.begin(), testvec.end(), order_.begin()) == 1);
    }

    void test_protected() {
        test_is_valid();
        test_is_in_range();
        test_mutate();
    }
};

int main() {
    std::stringstream ss;
    ss << "1 \t 1";
    ss << "33 \t 3";
    ss << "11 \t 123";
    
    Cities city;
    ss >> city;

    TestChromosome tc(&city);

    tc.test_protected();
}