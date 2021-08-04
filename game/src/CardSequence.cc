#include <CardSequence.h>

// no randomization
unsigned threes::game::alwaysReturnNextIndex(const unsigned lower, const unsigned upper) {
  (void)upper; // unused, this is to preserve interface 
  return lower;
}

// a standard default impl that does uniform shuffle 
unsigned threes::game::uniformRandomIndex(const unsigned lower, const unsigned upper) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<unsigned> dist(lower, upper);
  return( dist(g) );
}

threes::game::ShuffleDeckContents threes::game::threesDefaultShuffleDeck() {
  threes::game::ShuffleDeckContents result = {
    Card(1), Card(1), Card(1), Card(1),
    Card(2), Card(2), Card(2), Card(2),
    Card(3), Card(3), Card(3), Card(3) };
  return result;
}

threes::game::ShuffleDeckContents threes::game::oneTwoThreeDeck() {
  threes::game::ShuffleDeckContents result = { Card(1), Card(2), Card(3) };
  return result;
}
