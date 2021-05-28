#pragma once

/*
 * Class representing the board state
 */

#include <array>

#include "Card.h"

namespace threes {
  namespace game {

    enum ShiftDirection {
     DIRECTION_UP,
     DIRECTION_DOWN,
     DIRECTION_LEFT,
     DIRECTION_RIGHT
    };
    
    template<unsigned DIM>
    class Board {

    public:
      using storage_t = std::array<Card,DIM*DIM>;
      
    public:
      Board();

      // const access to underlying data
    public:
      const storage_t& underlyingDataRef() const {
	return m_data; 
      }

      const Card& cardAtIndex(const unsigned row, const unsigned col) const {
	return matrixIndex(row, col);
      }

      // utilites for actually playing the game  
    public:
      bool canShiftVertical() const;
      bool canShiftHorizontal() const;

      void shiftBoard(const ShiftDirection dir, const Card insertVal);
      
    private:
      Card& matrixIndex(const unsigned row, const unsigned col) const {
	// todo: bounds check
	const unsigned arrIdx = col + row*DIM;
	return m_data[arrIdx];
      }

      // helper for finding a valid random index for inserting new card
      unsigned chooseInsertIndex(const ShiftDirection dir) const;

      // single impl for shifting an individual row or column, dedupes
      // the logic of figuring out what gets combined and what gets moved
      // choice of start/stride selects the relevant row/col and the direction
      void shiftSlice(const int startIdx, const int stride);

      bool canShiftSlice(const int startIdx, const int stride) const;
      
    private:
      
      std::array<Card,DIM*DIM> m_data;

    };


    ///////////////////////////////////////////////
    // Template Implementations
    ///////////////////////////////////////////////

    template<unsigned DIM>
    Board<DIM>::Board() {
      // for clarity/any potential future underlying changes,
      // this happens automatically with defaults
      std::fill(m_data.begin(), m_data.end(), Card(0));
    }

    /////////////////////
    
    template<unsigned DIM>
    bool Board<DIM>::canShiftVertical() const {
      for(int i=0; i<DIM; ++i) {
	if( canShiftSlice( i, static_cast<int>(DIM) ) ) {
	  return(true);
	}
      }
      return(false);
    }

    /////////////////////
    
    template<unsigned DIM>
    bool Board<DIM>::canShiftHorizontal() const {
      for(int i=0; i<DIM; ++i) {
	if( canShiftSlice( static_cast<int>(DIM)*i, 1 ) ) {
	  return(true);
	}
      }
      return(false);
    }

    ////////////////////
    
    template<unsigned DIM>
    void Board<DIM>::shiftSlice(const int startIdx, const int stride) {
      // todo: if( startIdx + stride*DIM > DIM*DIM ) { raise error };

      if(DIM < 2) { return; }
      
      // two parts here - part 1 is to find the first combinable pair of cards if any
      //                - part 2 is to shift all remaining cards by 1 stride

      // find first combinable cards, combine them, track the first
      // position where we will shift the remaining cards.
      // If that position is DIM, there was no valid card combination
      unsigned shiftDestination = DIM;
      for(unsigned i = 0; i < (DIM-1); ++i) {
	const unsigned currIdx = startIdx + i*stride;
	const unsigned nextIdx = startIdx + (i+1)*stride;

	Card& curr = m_data[currIdx];
	const Card& next = m_data[nextIdx];
	if( curr.canCombine(next) ) {
	  curr.value += next.value;
	  shiftDestination = i+1;
	}
	
      }

      // shift all remaining cards by 1 stride
      // if no combo found, (shiftDestination == DIM) and
      // this loop does not execute.
      for(; shiftDestination < (DIM-1); ++shiftDestination) {
	const unsigned destinationIdx = startIdx + shiftDestination*stride;
	const unsigned originIdx      = startIdx + (shiftDestination+1)*stride;

	m_data[destinationIdx] = m_data[originIdx];
      }

      // We didn't copy anything in to the shiftDestination == (DIM-1) case
      // because there is no data to copy from. We need to copy an empty
      // (zero value) card from off the board in to that spot
      if( shiftDestination == (DIM-1) ) {
	m_data[ startIdx + shiftDestination*stride ] = Card(0);
      }
      
      
    }

    /////////////////////

    
    template<unsigned DIM>
    bool Board<DIM>::canShiftSlice(const int startIdx, const int stride) const {
      // todo: if( startIdx + stride*DIM > DIM*DIM ) { raise error };

      if(DIM < 2) { return false; }

      // check if any two adjacent cards are combinable
      for(unsigned i=0; i<(DIM-1); ++i) {
	const unsigned currDataIdx = startIdx + i*stride;
	const unsigned nextDataIdx = startIdx + (i+1)*stride;
	const Card& curr = m_data[currDataIdx];
	const Card& next = m_data[nextDataIdx];

	if( curr.canCombine(next) ) {
	  return(true);
	}
      }

      return(false);
      
    }
    
  } // namespace game
} // namespace threes
