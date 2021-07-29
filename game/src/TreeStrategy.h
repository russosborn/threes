#pragma once

#include "Board.h"
#include "CardSequence.h"

#include "GameDriverStrategy.h"


namespace threes {
  namespace game {

    
    template<class BOARD>
    class ExpectiMaxTree : public IThreesStgy<BOARD> {
    public:
      virtual ShiftDirection move(const typename GameDriver<BOARD>::BoardPtr& boardPtr,
			      const typename ICardSequence<BOARD>::ICardSeqPtr& seqPtr) override {

	// for each direction in up, down, left right:
	//   for each possible drawn card (how to handle bonus card, could be very large search space)
	//       for each possible new card location:
	//          expectedScore(board, 
      }
    public:

      static std::array<unsigned, 3> getTopThreeValues(const typename BOARD::storage_t& rawBoardData) {

	std::array<unsigned, 3> topThree{ 0, 1, 2 };
	for( auto card : rawBoardData ) {
	  // check if this card needs to replace one of the 3 largest values
	  // todo: cleaner to just put new card in 3rd spot and std::sort?
	  if( card.value > topThree[0] &&
	      card.value != topThree[1] &&
	      card.value != topThree[2] ) {
	  
	    // if this card is top 1 or 2
	    if(card.value > topThree[1]) {
	      // shift two to three
	      topThree[0] = topThree[1];
	      // if this card is top 1
	      if(card.value > topThree[2]) {
		// shift 1 to 2, insert at 1
		topThree[1] = topThree[2];
		topThree[2] = card.value;
	      } else {
		// card not top 1, put it in second spot
		topThree[1] = card.value;
	      }
	    } else { // this card is not top 1 or 2
	      // put it in third posiiton
	      topThree[0] = card.value;
	    }
	  }
	}
	return(topThree);
      }

      static double addScoreLogic( const unsigned card1Val, const unsigned card2Val) {
	double score = 0.0;
	// exact match, check only to the right to not double count
	if( card1Val == card2Val ) {
	  score += 2.0;
	}
	// next to a card twice its value
	else if( card1Val == (2 * card2Val) ) {
	  score += 2.0;
	}
	// card1 is 2x next card's value
	else if( card2Val == (2 * card1Val) ) {
	  score += 2.0;
	}
	return score;
      }

      static bool nextToOtherVal(const unsigned row, const unsigned col,
				 const unsigned otherVal,
				 const typename BOARD::storage_t& data) {

	const unsigned i = col + BOARD::dim*row;
	bool result(false);
	if( col > 0 ) {
	  if( data[i-1].value == otherVal ) { result = true; }
	}
	if( col < BOARD::dim-1 ) {
	  if( data[i+1].value == otherVal ) { result = true; }
	}
	if( row > 0 ) {
	  if( data[i-BOARD::dim].value == otherVal ) { result = true; }
	}
	if( row < BOARD::dim-1 ) {
	  if( data[i+BOARD::dim].value == otherVal ) { result = true; }
	}
	return(result);
      }

      // some ideas for value functions here
      // https://nbickford.wordpress.com/2014/04/18/how-to-beat-threes-and-2048/
      virtual double valueFunction(const BOARD& board) {

	/* To start, this is the logic I'm stealing (from Walt Destler, I think)

	  Every empty space is worth 3 points.
	  Every matching pair of adjacent cards is worth 2 points.
	  A card next to another card twice its value is worth 2 points.
	  A card trapped between two other cards of higher value, 
                 or between a wall and a card of higher value, is penalized 5 points.
	  Cards of the second-largest size get a bonus of 1 point 
                 if they’re next to the largest card, and an extra point if they’re next to a wall.
	  Cards of the third-largest size get a bonus of 1 point
                  if they’re next to a wall and are next to a card of the second-largest size.
	  The largest card gets a +3 bonus if it’s next to one wall, or a +6 bonus if it’s in a corner.
	*/
	double score(0.0);
	auto rawBoardData(board.underlyingDataRef());
	const std::array<unsigned, 3> topThree( getTopThreeValues( rawBoardData ) );

	// check for empties everywhere
	for( auto card : rawBoardData ) {
	  if(card.value == 0) { score += 3.0; }
	}

	// check matching adjacent cards, card next to card 2x its value
	unsigned row=0, col=0;
	for(unsigned i = 0; i < rawBoardData.size(); ++i) {

	  const unsigned currCardVal = rawBoardData[i].value;
	  // if current square is empty, skip all the other bonuses
	  if( currCardVal != 0) { 

	    // check down+right so as to not double count
	    if( col < BOARD::dim - 1) { // check across columns
	      const unsigned neighborCardVal = rawBoardData[i+1].value;
	      score += addScoreLogic(currCardVal, neighborCardVal);
	    }
	    if( row < BOARD::dim - 1) { // check across rows
	      const unsigned neighborCardVal = rawBoardData[i+BOARD::dim].value;
	      score += addScoreLogic(currCardVal, neighborCardVal);
	    }
	    //
	    // "Trapped" penalties
	    const bool trappedLeft (col == 0            || rawBoardData[i-1].value > currCardVal);
	    const bool trappedRight(col == BOARD::dim-1 || rawBoardData[i+1].value > currCardVal);
	    const bool trappedAbove(row == 0            || rawBoardData[i-BOARD::dim].value > currCardVal);
	    const bool trappedBelow(row == BOARD::dim-1 || rawBoardData[i+BOARD::dim].value > currCardVal);

	    if( trappedLeft  && trappedRight ) { score -= 5.0; }
	    if( trappedAbove && trappedBelow ) { score -= 5.0; }
	    //
	    // extra bonuses for largest/second largest/third largest special conditions
	    const bool nextToWall = (row == 0 || col == 0 || row == BOARD::dim-1 || col == BOARD::dim-1);
	    const bool inCorner   = ((row == 0 || row == BOARD::dim-1) &&
				     (col == 0 || col == BOARD::dim-1)   );
	    if( currCardVal == topThree[0] && nextToWall ) { // third largest
	      if(nextToOtherVal(row, col, topThree[1], rawBoardData)) { score += 1.0; }
	    }
	    else if( currCardVal == topThree[1] ) { // second largest
	      if(nextToWall) { score += 1.0; }
	      if(nextToOtherVal(row,col,topThree[2], rawBoardData)) { score += 1.0; }
	    }
	    else if( currCardVal == topThree[2] ) { // largest
	      if(        inCorner ) { score += 6.0; }
	      else if( nextToWall ) { score += 3.0; }
	    }

	  } // end "if card != 0"
	  
	  // update the "matrix" indices
	  ++col;
	  if(col == BOARD::dim) { col = 0; ++row; }
	}
	return(score);
      }
	
      double expectedValue( const BOARD& board, const ICardSequence<BOARD>& seq,
			    const ShiftDirection move, const unsigned depth ) {

	// recursive base case, if no more depth required, just return the
	// best guess of the value of the board
	if(depth == 0) {
	  return(valueFunction(board));
	}
	  
	static constexpr std::array<ShiftDirection, NUM_DIRECTIONS>
	  candidateMoves{ DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT};
	  
	//   copy state to state'
	BOARD boardCopy(board);
	ICardSequence<BOARD> seqCopy(seq);

	//   update state' with move
	ASSERT( boardCopy.canShift(move), "invalid shift request in EV calc");
	Card insertCard(seqCopy.draw(boardCopy));
	boardCopy.shiftBoard(move, insertCard);

	// for each valid move, determine expected value
	// of that move recursively
	unsigned numValidMoves(0);
	double accumulatedScore(0.0);
	for(auto candidateMove : candidateMoves) {
	  if( boardCopy.canShift(candidateMove) ) {
	    ++numValidMoves;
	    accumulatedScore += expectedValue(std::make_pair(boardCopy, seqCopy),
					      candidateMove, depth-1);
	  }
	}

	return accumulatedScore / static_cast<double>(numValidMoves);
      }
      
    }; // class ExpectiMaxTree


  } // ns game
} // ns threes
