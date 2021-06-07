#pragma once

/*
 *
 */

namespace threes {
  namespace game {


    // for the moment, obey the Threes convention of 1/2, then 2^n*3
    // Would be slightly simpler logic below if we relabelled and used
    // powers of two, but whatever.
    struct Card {
       // type out the constructor for clarity's sake
      Card(const unsigned initVal) : value(initVal) {}
      Card() : value(0) {}
      
      unsigned value;

      bool canCombine(const Card& other) const {
	bool isEmpty = ((value == 0) || (other.value==0));
	bool isBaseCombo = ( (value + other.value) == 3 );
	bool isMatch = (value > 2 && value == other.value);

	return (isEmpty || isBaseCombo || isMatch);
      }

      bool operator> (const Card& other)  const { return value > other.value; }
      bool operator> (const int otherVal) const { return value > otherVal;  }
      bool operator==(const Card& other)  const { return value == other.value; }
    };

    
  } //namespace game
} //namespace threes
