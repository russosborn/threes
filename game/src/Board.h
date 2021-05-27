#pragma once

/*
 * Class representing the board state
 */

#include <array>

#include "Card.H"

namspace threes {
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
      class Board();

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
      
    private:
      
      std::array<Card,DIM*DIM> m_data;

    };
  } // namespace game
} // namespace threes
