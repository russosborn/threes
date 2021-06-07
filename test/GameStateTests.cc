#include <src/Board.h>
#include <src/Card.h>
#include <gtest/gtest.h>


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



// Demonstrate some basic assertions.
TEST(BoardState, BasicAssertions) {
  threes::game::Board<3,AlwaysGenerateMinVal> testBoard; // starts as 3x3 of value 0

  // should add Card(1)
  const threes::game::Card cardOne(1);
  const threes::game::Card cardTwo(2);
  const threes::game::Card cardThree(3);
  const threes::game::Card cardSix(6);

  // add 1 then 2 then 3 from the bottom.
  // Randgen should insert them in 1st column
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardTwo);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardThree);

  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(0,0) );
  EXPECT_EQ( cardTwo  , testBoard.cardAtIndex(1,0) );
  EXPECT_EQ( cardThree, testBoard.cardAtIndex(2,0) );

  // now if we shift the board up, the 1 at (0,0) and the 2 at (1,0)
  // should be combined
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardThree  , testBoard.cardAtIndex(0,0) );  
  EXPECT_EQ( cardThree  , testBoard.cardAtIndex(1,0) );  

  // now if we shift the board up again, the 3 at (0,0) and the 3 at (1,0)
  // should be combined
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardSix  , testBoard.cardAtIndex(0,0) );  

  // now column 1 cannot be combined
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(2,1) );
  
}
