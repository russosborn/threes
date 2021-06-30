#include <CardSequence.h>

// no randomization
unsigned threes::game::alwaysReturnNextIndex(const unsigned lower, const unsigned upper) {
  return lower;
}

// a standard default impl that does uniform shuffle 
unsigned threes::game::uniformRandomIndex(const unsigned lower, const unsigned upper) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<unsigned> dist(lower, upper);
  return( dist(g) );
}
