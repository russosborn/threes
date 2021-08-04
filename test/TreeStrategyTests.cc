#include <src/Board.h>
#include <src/Card.h>
#include <src/TreeStrategy.h>
#include <gtest/gtest.h>


// random generator that always returns min value in the
// random range for testing purposes
class AlwaysGenerateMinVal {
public:
  AlwaysGenerateMinVal(const int min, const int max)
    : m_min(min)
  {(void)max;}

  template<typename RAND_GEN>
  int operator()(RAND_GEN& rd) const { (void)rd; return(m_min); }
  
private:
  const int m_min;
};


using threes::game::Card;

// Demonstrate some basic assertions.
TEST(TreeStrategy, NextToOtherVal) {
  using BoardType = threes::game::Board<3, AlwaysGenerateMinVal>;
  using TreeStgy = threes::game::ExpectiMaxTree<BoardType>;
  
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  const threes::game::Card cardTwo(2);
  const threes::game::Card cardThree(3);
  const threes::game::Card cardSix(6);

  std::vector<Card> initialCards{cardOne,cardTwo,cardThree};
  std::vector<unsigned> leftColumn{0,3,6};
  BoardType testBoard(initialCards, leftColumn);

  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(0,0) );
  EXPECT_EQ( cardTwo  , testBoard.cardAtIndex(1,0) );
  EXPECT_EQ( cardThree, testBoard.cardAtIndex(2,0) );

  // Board is now
  // 1 0 0
  // 2 0 0
  // 3 0 0
  
  std::array<unsigned, 3> topThreeVals = TreeStgy::getTopThreeValues( testBoard.underlyingDataRef() );
  EXPECT_EQ( 1  , topThreeVals[0] );
  EXPECT_EQ( 2  , topThreeVals[1] );
  EXPECT_EQ( 3  , topThreeVals[2] );

  // check "nextToOtherVal" logic, true case
  // column 0
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,0,2, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,0,3, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,0,1, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,0,2, testBoard.underlyingDataRef()) );
  // column 1
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,1,1, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,1,2, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,1,3, testBoard.underlyingDataRef()) );
  // column 2
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,2,0, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,2,0, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,2,0, testBoard.underlyingDataRef()) );


  // false cases
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,0,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,0,3, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(1,0,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,0,1, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,0,3, testBoard.underlyingDataRef()) );

  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,1,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,1,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(1,1,3, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(1,1,1, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,1,2, testBoard.underlyingDataRef()) );

  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,2,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,2,1, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(1,2,3, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(1,2,1, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,2,2, testBoard.underlyingDataRef()) );

  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardSix);
  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(0,1) );
  EXPECT_EQ( cardTwo  , testBoard.cardAtIndex(1,1) );
  EXPECT_EQ( cardThree, testBoard.cardAtIndex(2,1) );
  EXPECT_EQ( cardSix  , testBoard.cardAtIndex(0,0) );

  // Board is now
  //  6 1 0
  //  0 2 0
  //  0 3 0
  topThreeVals = TreeStgy::getTopThreeValues( testBoard.underlyingDataRef() );
  EXPECT_EQ( 2  , topThreeVals[0] );
  EXPECT_EQ( 3  , topThreeVals[1] );
  EXPECT_EQ( 6  , topThreeVals[2] );

  
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,0,1, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,1,6, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,1,2, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,1,1, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(1,1,3, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,0,3, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(0,2,1, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,1,2, testBoard.underlyingDataRef()) );
  EXPECT_TRUE( TreeStgy::nextToOtherVal(2,2,3, testBoard.underlyingDataRef()) );
  
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,2,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,2,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,2,1, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,0,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,0,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,0,3, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,0,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(2,0,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,2,2, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,2,6, testBoard.underlyingDataRef()) );
  EXPECT_FALSE( TreeStgy::nextToOtherVal(0,2,3, testBoard.underlyingDataRef()) );

}

TEST(TreeStrategy, AddScoreLogic) {
  using BoardType = threes::game::Board<3, AlwaysGenerateMinVal>;
  using TreeStgy = threes::game::ExpectiMaxTree<BoardType>;
  
  const unsigned cardOne(1);
  const unsigned cardTwo(2);
  const unsigned cardThree(3);
  const unsigned cardSix(6);
  const unsigned card12(12);
  const unsigned card24(24);

  EXPECT_EQ( TreeStgy::addScoreLogic( cardOne  , cardOne ), 2.0 );
  EXPECT_EQ( TreeStgy::addScoreLogic( cardSix  , card12  ), 2.0 );
  EXPECT_EQ( TreeStgy::addScoreLogic( card12   , cardSix ), 2.0 );
  EXPECT_EQ( TreeStgy::addScoreLogic( cardSix  , card24  ), 0.0 );
  EXPECT_EQ( TreeStgy::addScoreLogic( cardOne  , cardTwo ), 2.0 ); 
  EXPECT_EQ( TreeStgy::addScoreLogic( cardThree, cardTwo ), 0.0 ); 
  EXPECT_EQ( TreeStgy::addScoreLogic( cardOne  , cardTwo ), 2.0 ); 
}

// Demonstrate some basic assertions.
TEST(TreeStrategy, TotalScore) {
  using BoardType = threes::game::Board<3, AlwaysGenerateMinVal>;
  using TreeStgy = threes::game::ExpectiMaxTree<BoardType>;
  
  const threes::game::Card cardZero(0);
  const threes::game::Card cardOne(1);
  const threes::game::Card cardTwo(2);
  const threes::game::Card cardThree(3);
  const threes::game::Card cardSix(6);
  const threes::game::Card card12(12);

  std::vector<Card> initialCards{cardOne, cardTwo, cardThree};
  std::vector<unsigned> diagonal{0,4,8};
  BoardType testBoard(initialCards, diagonal);

  EXPECT_EQ( cardOne  , testBoard.cardAtIndex(0,0) );
  EXPECT_EQ( cardTwo  , testBoard.cardAtIndex(1,1) );
  EXPECT_EQ( cardThree, testBoard.cardAtIndex(2,2) );

  // Board is now
  // 1 0 0
  // 0 2 0
  // 0 0 3

  
  TreeStgy stgy(4, 100);

  double zeroScore = 3.0*6;
  double largeCornerScore = 6.0;
  EXPECT_EQ( stgy.valueFunction(testBoard), zeroScore+largeCornerScore );

  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, cardSix);

  // Board is now
  // 6 1 0 
  // 0 0 2
  // 0 0 3

  // 5 zeros
  // 3rd largest next to 2nd largest and wall + 1
  // 2nd largest next to wall + 1
  // largest in corner +6

  EXPECT_EQ( stgy.valueFunction(testBoard), 3.0*5 + 1.0 + 1.0 + 6.0 );

  testBoard.shiftBoard(threes::game::DIRECTION_RIGHT, card12);
  // Board is now
  // 12 6 1 
  // 0  0 2
  // 0  0 3

  // 4 zeros +12
  // 12 next to 6 +2
  // largest in corner +6
  // 2nd largest next to wall and largest +2
  // 1 next to 2
  // 1 is trapped against right wall and 6 -5
  // 1 is trapped against top and 3 -5
  EXPECT_EQ( stgy.valueFunction(testBoard), 3.0*4 + 2.0 + 6.0 + 2.0 + 2.0 - 5.0 - 5.0 );


  std::vector<Card> trappedCards{card12, cardTwo, card12};
  std::vector<unsigned> topRow{0,1,2};
  BoardType trapTop(trappedCards, topRow);

  // 12  2 12
  //  0  0  0
  //  0  0  0

  // 6 zeros (+18)
  // largest in corner times two (+12)
  // second largest against wall and next to largest +2.0
  // 2 trapped between 12s (-5)
  EXPECT_EQ( stgy.valueFunction(trapTop), 18.0 + 12.0 + 2.0 - 5.0);

  std::vector<unsigned> bottomRow{6,7,8};
  BoardType trapBottom(trappedCards, bottomRow);
  // same score as trapped top
  EXPECT_EQ( stgy.valueFunction(trapBottom), 18.0 + 12.0 + 2.0 - 5.0);

  std::vector<unsigned> rightCol{2,5,8};
  BoardType trapRight(trappedCards, rightCol);
  // same score as trapped top
  EXPECT_EQ( stgy.valueFunction(trapRight), 18.0 + 12.0 + 2.0 - 5.0);

  std::vector<unsigned> leftCol{0,3,6};
  BoardType trapLeft(trappedCards, leftCol);
  // same score as trapped top
  EXPECT_EQ( stgy.valueFunction(trapLeft), 18.0 + 12.0 + 2.0 - 5.0);

  // test trapping along the wall against a card in the middle
  std::vector<Card> trapTwo{cardTwo, cardThree};
  std::vector<unsigned> up{1,4};
  BoardType trapUp(trapTwo, up);

  // 0 2 0
  // 0 3 0
  // 0 0 0
  
  // 7 zeros +21
  // second largest against wall and next to largest +2
  // 2 trapped against wall
  
  EXPECT_EQ( stgy.valueFunction(trapUp), 21.0 + 2.0 - 5.0);

  std::vector<unsigned> down{7,4};
  BoardType trapDown(trapTwo, down);
  EXPECT_EQ( stgy.valueFunction(trapUp), 21.0 + 2.0 - 5.0);
  std::vector<unsigned> left{3,4};
  BoardType trapL(trapTwo, left);
  EXPECT_EQ( stgy.valueFunction(trapL), 21.0 + 2.0 - 5.0);
  std::vector<unsigned> right{5,4};
  BoardType trapR(trapTwo, right);
  EXPECT_EQ( stgy.valueFunction(trapR), 21.0 + 2.0 - 5.0);
  
}
