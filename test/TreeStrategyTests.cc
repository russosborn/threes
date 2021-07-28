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
  {}

  template<typename RAND_GEN>
  int operator()(RAND_GEN& rd) const { return(m_min); }
  
private:
  const int m_min;
};


using threes::game::Card;

// Demonstrate some basic assertions.
TEST(TreeStrategy, BasicAssertions) {
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

  

}

TEST(TreeStrategy, ScoreLogic) {
}
