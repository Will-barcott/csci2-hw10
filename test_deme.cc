#include "deme.hh"
#include "chromosome.hh"
#include <sstream>
#include <cassert>

class TestDeme : public Deme {
    public:
    TestDeme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
        : Deme(cities_ptr, pop_size, mut_rate)
        {}

    void test_get_best() {
        for (auto i : pop_) {
            i->get_fitness();
        }
        const Chromosome* c  = get_best();
        assert(c);
        double best = c->get_fitness();
        for (auto i : pop_) {
            assert(best >= i->get_fitness());
        }
    }

    void test_select_parent() {
        Chromosome* c = select_parent();
        assert(c);
    }

    void test_protected() {
        test_get_best();
        test_select_parent();
    }
};

int main() {
    std::stringstream ss;
    ss << "1 \t 11000\n";
    ss << "33 \t 3\n";
    ss << "11 \t 123\n";
    ss << "66 \t 4444";

    Cities city;
    ss >> city;

    std::cout.precision(10);
    TestDeme td(&city, 20, 0.4);
    td.test_protected();
}