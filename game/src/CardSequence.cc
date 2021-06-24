#include <CardSequence.h>

// shuffle algorithm that does nothing
void threes::game::nullShuffle(threes::game::ShuffleDeckContents& deck) {
  return;
}

// a standard default impl that does uniform shuffle 
void threes::game::uniformShuffle(threes::game::ShuffleDeckContents& deck) {
  std::random_device rd;
  std::mt19937 g(rd());
  
  std::shuffle(deck.begin(), deck.end(), g);
}
