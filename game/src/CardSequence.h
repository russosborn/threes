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
      using BoardPtrType = std::unique_ptr<BOARD_TYPE>;
      using CardSeqFactory = ro::ObjectFromStrFactory< ICardSequence<BOARD_TYPE> >;
      static CardSeqFactory s_factory;
      using ICardSeqPtr = typename CardSeqFactory::ObjectPtr;

    public:
      ICardSequence() {}
      virtual ~ICardSequence() {}
      virtual Card draw(const BoardPtrType& b) = 0; // remove top card
      virtual Card peek(const BoardPtrType& b) = 0; // peek at the top card

    public:
      virtual unsigned write_binary(std::ostream& out) const = 0;
    };

    /*
     * Collection ofFfree helper functions,  useful defaults for many shuffle algorithms, 
     */

    // BONUS CARD SELECTION
    
    // uniformly randomly select a Card with (b.maxCard()/S_BONUS_CARD_RATIO >= value > 3)
    // and value an exact power of 2 times six
    template<class BOARD_TYPE>
    Card genBonusCard(const std::unique_ptr<BOARD_TYPE>& b);


    // BASIC DECKS OF NEW CARDS DRAWN FOR INSERTION
    
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

    static ShuffleDeckContents oneTwoThreeDeck() {
      ShuffleDeckContents result = { Card(1), Card(2), Card(3) };
      return result;
    }

    // ALGORITHMS TO RANDOMLY SHUFFLE DECKS
    
    // shuffle algorithm that does nothing
    unsigned alwaysReturnNextIndex(const unsigned lower, const unsigned upper);

    // a standard default impl that does uniform shuffle 
    unsigned uniformRandomIndex(const unsigned lower, const unsigned upper);

    
    // BINARY PREDICATES TO DECIDE WHETHER OR NOT TO DRAW A BONUS CARD
    
    // never any bonus cards
    template<class T>
    bool alwaysFalse(const T&) { return false; }

    // internet's best guess
    template<typename BoardPtrType>
    bool defaultBonusDraw(const BoardPtrType& boardPtr) {
      if(!boardPtr) { return(false); }
      
      static const double s_randomOdds(1.0/21.0);
      
      if(boardPtr->maxCard() < S_BONUS_CARD_THRESHOLD) { return false;}
      std::random_device rd;
      std::mt19937 g(rd());
      
      std::uniform_real_distribution<> dist(0, 1.0);
      const double randZeroOne = dist(g);
      return(randZeroOne < s_randomOdds);
    }

    ///////////////////////////////
    
    template<class BOARD_TYPE>
    class Kamikaze28Sequence : public ICardSequence<BOARD_TYPE> {
      // public types to make deck contents and randomness generic 
    public:
      using typename ICardSequence<BOARD_TYPE>::BoardPtrType;
      
      // function that permutes a ShuffleDeckContents in place
      using IndexSelectFunction = std::function<unsigned(const unsigned, const unsigned)>;

      // function that decides whether or not to draw a bonus card
      using BonusCardDraw = std::function<bool(const BoardPtrType&)>;

      static typename ICardSequence<BOARD_TYPE>::ICardSeqPtr create(const std::string& cfg);
      
    public:
      Kamikaze28Sequence(const ShuffleDeckContents& deck,
			 IndexSelectFunction idxSelect = uniformRandomIndex,
			 BonusCardDraw bonusDraw = defaultBonusDraw<BoardPtrType>);

      // ICardSequence interface
    public:
      virtual Card draw(const BoardPtrType& b) override;
      virtual Card peek(const BoardPtrType& b) override;

    public:
      // todo:: could optionally expose more state, e.g. what cards are still in the deck
      // prior to next shuffle
      virtual unsigned write_binary(std::ostream& out) const override {
	if(!out.good()) { return 0; }
	out << m_next.value;
	return(sizeof( decltype(m_next.value) ));
      }

    private:
      void setupNextCard();
      
    private:
      ShuffleDeckContents m_deck;
      unsigned m_deckIdx; 
      Card m_next;

      IndexSelectFunction m_indexSelect;
      BonusCardDraw m_bonusDraw;
    };

    ////////////////////////////////////////////////////////////////
    // template impls
    ////////////////////////////////////////////////////////////////

    // todo: make this generic but also performant
    template<class BOARD_TYPE>
    Card genBonusCard(const std::unique_ptr<BOARD_TYPE>& bPtr) {
      // todo: make this generic for non-3 based values
      static std::vector<Card> s_bonusCards; // sorted possible bonus card values

      ASSERT( !(S_BONUS_CARD_THRESHOLD > bPtr->maxCard().value),
		 "board does not meet special card threshold");
	
      const Card maxBonusCard = bPtr->maxCard().value / S_BONUS_CARD_RATIO;
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
    typename ICardSequence<BOARD_TYPE>::ICardSeqPtr
    Kamikaze28Sequence<BOARD_TYPE>::create(const std::string& cfg) {
      if(cfg == "default") {
	return typename ICardSequence<BOARD_TYPE>::ICardSeqPtr(
	       new Kamikaze28Sequence<BOARD_TYPE>(threesDefaultShuffleDeck()));
      } else if(cfg == "test") {
	// simplest deck is only 1,2,3
	// shuffle logic does nothing
	// provides deterministic card sequencing, e.g. for tests
	
	return typename ICardSequence<BOARD_TYPE>::ICardSeqPtr(
	  new Kamikaze28Sequence<BOARD_TYPE>(oneTwoThreeDeck(),
					     alwaysReturnNextIndex,
					     alwaysFalse<BoardPtrType>));
	
      }
      else {
	ASSERT(false, "invalid k28seq config, only default/test supported");
      }

      return(nullptr);
    }

    /////////////////////////////////////////
    
    template<class BOARD_TYPE>
    Kamikaze28Sequence<BOARD_TYPE>::Kamikaze28Sequence(const ShuffleDeckContents& deck,
						       IndexSelectFunction idxSelect,
						       BonusCardDraw bonusDraw)
      : ICardSequence<BOARD_TYPE>()
      , m_deck(deck)
      , m_deckIdx(0)
      , m_indexSelect(idxSelect)
      , m_bonusDraw(bonusDraw)
      {
	setupNextCard();
      }


    
    /////////////////////////////////////////
    
    // All the work happens here - we already know
    // we're returning the "next" card we have saved,
    // but updating "next" is the challenge
    template<class BOARD_TYPE>
    Card Kamikaze28Sequence<BOARD_TYPE>::draw(const BoardPtrType& b) {
      Card result = m_next;

      // draw bonus or from deck?
      if( m_bonusDraw(b) ) {
	m_next = genBonusCard(b);
      }
      else {
	// pick a remaining card at random
	setupNextCard();
      }

      return(result);
    }

    /////////////////////////////////////////

    template<class BOARD_TYPE>
    Card Kamikaze28Sequence<BOARD_TYPE>::peek(const BoardPtrType& b) { return(m_next); }


    template<class BOARD_TYPE>
    void Kamikaze28Sequence<BOARD_TYPE>::setupNextCard() {
      const unsigned nextSelectedIdx =
	m_indexSelect(m_deckIdx, m_deck.size());

      // move the selected card to the top of the deck
      std::swap( m_deck[m_deckIdx], m_deck[nextSelectedIdx] );

      // pick the selected card off the top of the deck
      m_next = m_deck[m_deckIdx];
      ++m_deckIdx;

      // if at the end of the deck, start over
      if(m_deckIdx == m_deck.size()) {
	m_deckIdx = 0;
      }
    }

    
  } // ns game
} // ns threes
