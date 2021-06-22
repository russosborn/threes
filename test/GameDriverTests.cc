#include <gtest/gtest.h>

#include <src/GameDriver.h>
#include <src/Board.h>
#include <iostream>

// random generator that always returns min value in the
// random range for testing purposes
class AlwaysGenerateMinVal {
public:
  AlwaysGenerateMinVal(const int min, const int max)
    : m_min(min)
  {}

  template<typename RAND_GEN>
  int operator()(RAND_GEN& rd) const { return(m_min); }
  
private:
  const int m_min;
};


using threes::game::Card;
using TestBoard = threes::game::Board<3, AlwaysGenerateMinVal>;

// Demonstrate some basic assertions.
TEST(GameDriver, Basic) {
  threes::game::ICardSequence<TestBoard>::s_factory.registerCreator(
  "k28d",
  threes::game::Kamikaze28Sequence<TestBoard>::create);

  std::unique_ptr<threes::game::GameDriver<TestBoard>> game(
    new threes::game::GameDriverIStream<TestBoard>("k28d", "default", 6, std::cin) );
}
