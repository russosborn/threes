#pragma once

#include "Card.h"
#include "Board.h"
#include "Utils.h"

#include <vector>
#include <random>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace threes {
  namespace game {

    // todo: could make this a generic per-sequence variable
    static constexpr Card S_BONUS_CARD_THRESHOLD(48);
    static constexpr int S_BONUS_CARD_RATIO(8);
    
    // abstract base to generate a sequence of Card values
    template<class BOARD_TYPE>
    class ICardSequence {
    public:
      ICardSequence() {}
      virtual ~ICardSequence() {}
      virtual Card draw(const BOARD_TYPE& b) = 0; // remove top card
      virtual Card peek(const BOARD_TYPE& b) = 0; // peek at the top card
    };

    // uniformly randomly select a Card with (b.maxCard()/S_BONUS_CARD_RATIO >= value > 3)
    // and value an exact power of 2 times six
    template<class BOARD_TYPE>
    Card genBonusCard(const BOARD_TYPE& b);
    
    // Implementation of the internet's best guess 
    // https://toucharcade.com/community/threads/threes-by-sirvo-llc.218248/page-27#post-3140044

    using ShuffleDeckContents = std::vector<Card>;
    static ShuffleDeckContents threesDefaultShuffleDeck() {
      ShuffleDeckContents result = {
        Card(1), Card(1), Card(1), Card(1),
	Card(2), Card(2), Card(2), Card(2),
	Card(3), Card(3), Card(3), Card(3) };
      return result;
    }

    template<class BOARD_TYPE>
    class Kamikaze28Sequence : public ICardSequence<BOARD_TYPE> {
      // public types to make deck contents and randomness generic 
    public:
      // function that permutes a ShuffleDeckContents in place
      using ShuffleFunction = std::function<void(ShuffleDeckContents&)>;

      // function that decides whether or not to draw a bonus card
      using BonusCardDraw = std::function<bool(const BOARD_TYPE&)>;

      // a standard default impl that does uniform shuffle 
      static inline void uniformShuffle(ShuffleDeckContents& deck) {
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(deck.begin(), deck.end(), g);
      }

      static inline bool defaultBonusDraw(const BOARD_TYPE& board) {
	static const double s_randomOdds(1.0/21.0);
	  
	if(board.maxCard() < S_BONUS_CARD_THRESHOLD) { return false;}
	std::random_device rd;
	std::mt19937 g(rd());

	std::uniform_real_distribution<> dist(0, 1.0);
	const double randZeroOne = dist(g);
	return(randZeroOne < s_randomOdds);
      }
      
    public:
      Kamikaze28Sequence(const ShuffleDeckContents& deck,
			 ShuffleFunction shuffFunc = uniformShuffle,
			 BonusCardDraw bonusDraw = defaultBonusDraw);

      // ICardSequence interface
    public:
      virtual Card draw(const BOARD_TYPE& b) override;
      virtual Card peek(const BOARD_TYPE& b) override;

    private:
      ShuffleDeckContents m_deck;
      unsigned m_deckIdx; 
      Card m_next;

      ShuffleFunction m_shuffleFunc;
      BonusCardDraw m_bonusDraw;
    };

    ////////////////////////////////////////////////////////////////
    // template impls
    ////////////////////////////////////////////////////////////////

    // todo: make this generic but also performant
    template<class BOARD_TYPE>
    Card genBonusCard(const BOARD_TYPE& b) {
      // todo: make this generic for non-3 based values
      static std::vector<Card> s_bonusCards; // sorted possible bonus card values

      ASSERT( !(S_BONUS_CARD_THRESHOLD > b.maxCard().value),
		 "board does not meet special card threshold");
	
      const Card maxBonusCard = b.maxCard().value / S_BONUS_CARD_RATIO;
      if(s_bonusCards.empty()) {
	s_bonusCards.push_back(Card(S_BONUS_CARD_THRESHOLD.value/S_BONUS_CARD_RATIO));
      }

      Card currMaxCard = s_bonusCards.back();
      
      while( currMaxCard < maxBonusCard ) {
	currMaxCard = Card( currMaxCard.value * 2 );
	s_bonusCards.push_back(currMaxCard);
      }

      if( currMaxCard != maxBonusCard ) {
	std::cerr << "max bonus card not an exact power of two "
		  << "times 6: " << maxBonusCard.value << std::endl;
	ASSERT( false, "max bonus card not an exact match!" );
      }

      std::random_device rd;
      std::mt19937 g(rd());

      ASSERT( s_bonusCards.size() > 0, "no bonus cards to draw");
      
      std::uniform_int_distribution<> dist(0, s_bonusCards.size()-1);
      return(s_bonusCards[dist(g)]);
    }

    /////////////////////////////////////////

    template<class BOARD_TYPE>
    Kamikaze28Sequence<BOARD_TYPE>::Kamikaze28Sequence(const ShuffleDeckContents& deck,
						       ShuffleFunction shuffFunc,
						       BonusCardDraw bonusDraw)
      : ICardSequence<BOARD_TYPE>()
      , m_deck(deck)
      , m_deckIdx(0)
      , m_shuffleFunc(shuffFunc)
      , m_bonusDraw(bonusDraw)
      {
	  m_shuffleFunc(m_deck);
	  m_next = m_deck[m_deckIdx];
	  ++m_deckIdx;
	}


    
    /////////////////////////////////////////
    
    // All the work happens here - we already know
    // we're returning the "next" card we have saved,
    // but updating "next" is the challenge
    template<class BOARD_TYPE>
    Card Kamikaze28Sequence<BOARD_TYPE>::draw(const BOARD_TYPE& b) {
      Card result = m_next;

      // draw bonus or from deck?
      if( m_bonusDraw(b) ) {
	m_next = genBonusCard(b);
      }
      else {
	// check if at the end of the deck and need shuffle
	if(m_deckIdx == m_deck.size()) {
	  m_shuffleFunc(m_deck);
	  m_deckIdx = 0;
	}
	m_next = m_deck[m_deckIdx];
	++m_deckIdx;
      }

      return(result);
    }

    /////////////////////////////////////////

    template<class BOARD_TYPE>
    Card Kamikaze28Sequence<BOARD_TYPE>::peek(const BOARD_TYPE& b) { return(m_next); }

    
  } // ns game
} // ns threes
