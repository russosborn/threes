#include <src/CardSequence.h>
#include <src/Board.h>
#include <src/Card.h>
#include <gtest/gtest.h>

#include <set>

using Card = threes::game::Card;

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


// never any bonus cards
template<class T>
bool alwaysFalse(const T&) { return false; }

// preserve deck order
void nullShuffle(threes::game::ShuffleDeckContents& c) {
  return;
}


TEST(CardSequenceK28, SequenceTests) {
  using BoardType = threes::game::Board<3, AlwaysGenerateMinVal>;
  std::vector<Card> emptyInitialCards;
  BoardType unusedBoard(emptyInitialCards);
  
  threes::game::ShuffleDeckContents simpleContents{ Card(1), Card(2), Card(3) };
  
  threes::game::Kamikaze28Sequence<BoardType>
    k28Seq( simpleContents, nullShuffle, alwaysFalse<BoardType>);

  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(1) );

  // run through the deck twice, checking we get the cards in our expected order
  // (shuffle is doing nothing, never getting a bonus card)
  Card top = k28Seq.draw(unusedBoard);
  EXPECT_EQ( top, Card(1) );  
  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(2) );

  top = k28Seq.draw(unusedBoard);
  EXPECT_EQ( top, Card(2) );  
  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(3) );

  top = k28Seq.draw(unusedBoard);
  EXPECT_EQ( top, Card(3) );  
  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(1) );

  top = k28Seq.draw(unusedBoard);
  EXPECT_EQ( top, Card(1) );  
  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(2) );

  top = k28Seq.draw(unusedBoard);
  EXPECT_EQ( top, Card(2) );  
  EXPECT_EQ( k28Seq.peek(unusedBoard), Card(3) );
}

TEST(DefaultBonusGen, BonusCardTests) {
  using BoardType = threes::game::Board<10, AlwaysGenerateMinVal>;
  std::vector<Card> singleOne{Card(1)};
  BoardType board(singleOne);

  threes::game::ShiftDirection dirUp(threes::game::DIRECTION_UP);
  board.shiftBoard(dirUp, Card(48)); // only one possible card, 6
  
  EXPECT_EQ(Card(6), threes::game::genBonusCard(board));
  EXPECT_EQ(Card(48), board.maxCard());

  board.shiftBoard(dirUp, Card(96)); // two possible cards, 6 or 12
  // this is a little dirty because we're testing randomness
  // by running over and over and confirming we get all expected values...
  std::set<Card> possibleCards{ Card(6), Card(12) };
  std::set<Card> cardsNotFound( possibleCards );
  for(unsigned i=0; i<50; ++i) {
    auto possItr = possibleCards.find(threes::game::genBonusCard(board));
    EXPECT_TRUE( possItr != possibleCards.end() );
    auto notFoundItr = cardsNotFound.find(threes::game::genBonusCard(board));
    if(notFoundItr != cardsNotFound.end() ){
      cardsNotFound.erase(notFoundItr);
    }
  }

  EXPECT_TRUE(cardsNotFound.empty());
  
  
  
}

