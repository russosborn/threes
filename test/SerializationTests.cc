#include <src/Board.h>
#include <src/Card.h>
#include <gtest/gtest.h>

#include <sstream>

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

// Demonstrate some basic assertions.
TEST(Serialization, BoardSerialization) {
  const threes::game::Card cardOne(1);
  const threes::game::Card cardTwo(2);

  std::vector<Card> singleInitialCard{cardOne};
  std::vector<unsigned> bottomLeftIdx{6};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(singleInitialCard, bottomLeftIdx);

  // test fixed insert loc of initial card value 1
  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(2,0) );

  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardTwo);
  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(2,1) );
  EXPECT_EQ( cardTwo  , testBoard.cardAtIndex(2,0) );
  
  std::stringstream serializeBuffer;
  testBoard.write_binary(serializeBuffer);

  std::vector<Card> noCards;
  std::vector<unsigned> noIdx;
  threes::game::Board<3,AlwaysGenerateMinVal>
    restoredBoard(noCards, noIdx);
  
  restoredBoard.read_binary(serializeBuffer);
    
  EXPECT_EQ( cardOne  , restoredBoard.cardAtIndex(2,1) );  
  EXPECT_EQ( cardTwo  , restoredBoard.cardAtIndex(2,0) );
  EXPECT_EQ( testBoard, restoredBoard );
}

