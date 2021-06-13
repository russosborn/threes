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
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  const threes::game::Card cardTwo(2);
  const threes::game::Card cardThree(3);
  const threes::game::Card cardSix(6);

  std::vector<Card> singleInitialCard{cardOne};
  std::vector<unsigned> bottomLeftIdx{6};
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(singleInitialCard, bottomLeftIdx);

  // test fixed insert loc of initial card value 1
  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(2,0) );
  
  // add 1 then 2 then 3 from the bottom.
  // Randgen should insert them in 1st column
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

  // now column 1 cannot be combined, other columns are all 0s 
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_UP) );
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_DOWN) );

  // Board is now
  // 6 0 0
  // 1 0 0
  // 1 0 0

  // shift everything to the left, only valid insert is bottom row
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardTwo);
  EXPECT_EQ( cardSix,  testBoard.cardAtIndex(0,1) ); // shifted first column now in second col
  EXPECT_EQ( cardOne,  testBoard.cardAtIndex(1,1) ); // shifted first column now in second col
  EXPECT_EQ( cardOne,  testBoard.cardAtIndex(2,1) ); // shifted first column now in second col
  EXPECT_EQ( cardTwo,  testBoard.cardAtIndex(0,0) ); // newly inserted
  
  // Board is now
  //  2 6 0 
  //  0 1 0 
  //  0 1 0 

  // shift up, no cards should be combined, middle column doesn't move
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);  
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(0,0) ); // newly inserted
  EXPECT_EQ( cardTwo, testBoard.cardAtIndex(1,0) ); // 
  EXPECT_EQ( cardSix, testBoard.cardAtIndex(0,1) ); // middle column can't shift
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(1,1) ); // middle column can't shift
  EXPECT_EQ( cardOne, testBoard.cardAtIndex(2,1) ); // middle column can't shift

}

TEST(BoardState, ShiftDown) {
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  std::vector<Card> twoInitialCard{cardOne,cardOne};
  std::vector<unsigned> topRightIdxs{1,2}; // row 0, columns 1 and 2
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(twoInitialCard, topRightIdxs);

  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 

  // could insert in column 1, 2, "randomizer" picks col 1
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) );

  // Board is now
  //  0 1 0 
  //  0 1 0 
  //  0 1 1 
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_DOWN) );
  EXPECT_TRUE( testBoard.canShift(threes::game::DIRECTION_UP) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(2,2) ); 
}


TEST(BoardState, ShiftUp) {
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  std::vector<Card> twoInitialCard{cardOne,cardOne};
  std::vector<unsigned> bottomRightIdxs{3*3-2,3*3-1}; // row 2, columns 1 and 2
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(twoInitialCard, bottomRightIdxs);

  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) ); 

  // could insert in column 1, 2, randomizer picks col 1
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_UP, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) );

  // Board is now
  //  0 1 1 
  //  0 1 0 
  //  0 1 0 
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_UP) );
  EXPECT_TRUE( testBoard.canShift(threes::game::DIRECTION_DOWN) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_DOWN, cardOne);
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(2,2) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,2) ); 
}

TEST(BoardState, ShiftRight) {
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  std::vector<Card> twoInitialCard{cardOne,cardOne};
  std::vector<unsigned> bottomLeftIdxs{3,6}; // col 0, row 1 and 2
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(twoInitialCard, bottomLeftIdxs);

  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,0) ); 

  // could insert in row 1, 2, randomizer picks col 1
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(2,2) );

  // Board is now
  //  0 0 0 
  //  1 1 1 
  //  0 0 1 
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_RIGHT) );
  EXPECT_TRUE( testBoard.canShift(threes::game::DIRECTION_LEFT) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(2,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(2,2) ); 
}


TEST(BoardState, ShiftLeft) {
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  std::vector<Card> twoInitialCard{cardOne,cardOne};
  std::vector<unsigned> topRightIdxs{2,5}; // col 2, row 1 and 2
  threes::game::Board<3,AlwaysGenerateMinVal>
    testBoard(twoInitialCard, topRightIdxs);

  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,2) ); 

  // could insert in row 1, 2, randomizer picks col 1
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  testBoard.shiftBoard(threes::game::DIRECTION_LEFT, cardOne);
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,1) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(0,2) );
  EXPECT_EQ( cardOne,testBoard.cardAtIndex(1,0) );

  // Board is now
  //  1 1 1 
  //  1 0 0 
  //  0 0 0 
  EXPECT_FALSE( testBoard.canShift(threes::game::DIRECTION_LEFT) );
  EXPECT_TRUE( testBoard.canShift(threes::game::DIRECTION_RIGHT) );
  
  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardOne);
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,0) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,1) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(0,2) ); 
  EXPECT_EQ( cardZero,testBoard.cardAtIndex(1,2) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,0) ); 
  EXPECT_EQ( cardOne ,testBoard.cardAtIndex(1,1) ); 
}
