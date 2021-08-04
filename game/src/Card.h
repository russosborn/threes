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
      constexpr Card(const unsigned initVal) : value(initVal) {}
      Card() : value(0) {}
      
      unsigned value;

      bool canCombine(const Card& other) const {
	//bool isEmpty = ((value == 0) || (other.value==0));

	// can't combine a 0 on to anything
	if(other.value == 0) { return false; }
	
	// can shift anything on to a 0, but can't
	// shift a zero on to other cards
	bool isEmpty = (value == 0 && other.value > 0);
	// 1 and 2 combine 
	bool isBaseCombo = ( (value + other.value) == 3 );
	// otherwise, can only combine if value agrees
	bool isMatch = (value > 2 && value == other.value);

	return (isEmpty || isBaseCombo || isMatch);
      }

      bool operator> (const Card& other)  const { return value > other.value; }
      bool operator> (const int otherVal) const { return (otherVal < 0 || (value > static_cast<unsigned>(otherVal)));  }
      bool operator< (const Card& other)  const { return value < other.value; }
      bool operator< (const int otherVal) const { return (value < static_cast<unsigned>(otherVal) && otherVal > 0) ;  }
      bool operator==(const Card& other)  const { return value == other.value; }
      bool operator!=(const Card& other)  const { return value != other.value; }
    };

    double standardCardScore(const Card cardData);
    
  } //namespace game
} //namespace threes
