#include <algorithm> // for shuffle

#include "Board.h"

threes::game::Board<5> b(std::vector<threes::game::Card>{});

std::vector<unsigned> threes::game::pickNRandomIndicies(const unsigned n,
							const unsigned dim) {
  // shuffle the indices 0 .. (DIM*DIM)-1 and select the first numStartCards of them
  // to place the initial cards.
  std::vector<unsigned> oneToN(dim*dim);
  std::iota( std::begin(oneToN), std::end(oneToN), 0);
  
  // Inefficient if DIM*DIM very big and numStartCards small, but practical for
  // reasonable cases and only called once per game.
  std::shuffle(std::begin(oneToN), std::end(oneToN),
	       std::mt19937{ std::random_device{}() } );
  
  std::vector<unsigned> result(n);
  std::copy( oneToN.begin(), oneToN.begin()+n, result.begin() );
  
  return(result);
}
