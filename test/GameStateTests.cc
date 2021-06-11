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


using threes::game::Card;

// Demonstrate some basic assertions.
TEST(BoardState, BasicAssertions) {
  std::vector<Card> emptyInitialCards{};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(emptyInitialCards); // starts as 3x3 of value 0

  const threes::game::Card cardZero(0);
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

  // Board is now
  // 6 0 0
  // 1 0 0
  // 1 1 0

  // shift everything to the left, should be inserted in first row
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(2,0) ); // (2,0) does not combine
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(2,1) ); // (2,1) does not combine
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(0,2) ); // (0,2) is new insertion pt

  // shift left again to add a 2
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardTwo);
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardTwo, testBoard.cardAtIndex(0,2) ); 

  // Board is now
  // 6 1 2
  // 1 0 0
  // 1 1 0

  // shift to the right this time, combining 1+2 at (0,1) and (0,2)
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardTwo);
  EXPECT_EQ( cardThree, testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardSix, testBoard.cardAtIndex(0,1) ); // shifted first column now in second col
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(1,1) ); // shifted first column now in second col
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(2,1) ); // shifted first column now in second col
  EXPECT_EQ( cardTwo, testBoard.cardAtIndex(0,0) ); // newly inserted 2

  // shift down, no cards should be combined, middle column doesn't move
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardTwo);  
  EXPECT_EQ( cardThree,testBoard.cardAtIndex(1,2) ); // right column
  EXPECT_EQ( cardZero, testBoard.cardAtIndex(0,2) ); // right column
  EXPECT_EQ( cardSix,  testBoard.cardAtIndex(0,1) ); // middle column can't shift down
  EXPECT_EQ( cardOne,  testBoard.cardAtIndex(1,1) ); // middle column can't shift down
  EXPECT_EQ( cardOne,  testBoard.cardAtIndex(2,1) ); // middle column can't shift down
  EXPECT_EQ( cardTwo,  testBoard.cardAtIndex(0,0) ); // newly inserted 2
  EXPECT_EQ( cardTwo,  testBoard.cardAtIndex(1,0) ); // pvsly inserted 2
}

TEST(BoardState, ShiftDown) {
  std::vector<Card> emptyInitialCards{};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(emptyInitialCards); // starts as 3x3 of value 0

  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) );
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(2,0) );

  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) );
  
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 

  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) ); 

}

TEST(BoardState, ShiftUp) {
  std::vector<Card> emptyInitialCards{};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(emptyInitialCards); // starts as 3x3 of value 0

  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) );

  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) );
  
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 

  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) ); 

}

TEST(BoardState, ShiftLeft) {
  std::vector<Card> emptyInitialCards{};
  threes::game::Board<3,AlwaysGenerateMinVal>
        testBoard(emptyInitialCards); // starts as 3x3 of value 0

  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) );

  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) ); 

}

TEST(BoardState, ShiftRight) {
  std::vector<Card> emptyInitialCards{};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(emptyInitialCards); // starts as 3x3 of value 0

  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) );
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(0,2) );

  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) ); 

}
