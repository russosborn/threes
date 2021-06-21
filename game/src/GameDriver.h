#pragma once

#include "Board.h"
#include "Card.h"
#include "CardSequence.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <random>
#include <string>

namespace threes {
  namespace game {

    enum MoveResult {
      MOVE_VALID,
      MOVE_INVALID,
      END_GAME
    };

    // plays the game, renders things in ASCII and receives input from stdin
    template<class BOARD>
    class GameDriver {
    public:
      using BoardPtr = std::unique_ptr<BOARD> ;
      using CardSequencePtr =  ICardSqeuence<BOARD>::ICardSeqPtr;
      
    public:
      GameDriver(BoardPtr boardPtr,
		 CardSequencePtr sequencePtr,
		 const unsigned numStartCards );

      void play();

    public:
      static constexpr unsigned StateSize = BOARD::StateSize; 
      
    protected:
      virtual void render() const;

      MoveResult move(const ShiftDirection dir); 

      uint64_t gameScore() const;

      
    private:
      BoardPtr m_boardPtr;
      CardSequencePtr m_cardSeqPtr;

      

      
    }; // class GameDriver


    /////////////////////////////////////
    template<class BOARD>
    GameDriver<BOARD>::GameDriver(const std::string& sequencerType,
				  const unsigned numStartCards)
      : m_cardSeqPtr(ICardSequence<BOARD>::s_factory.create(sequencerType))
      {
	std::vector<Card> initialCards(numStartCards);
	for(auto itr = initalCards.begin(), itr != initialCars.end(); ++itr) {
	  *itr = m_cardSeqPtr->draw(nullptr); // null board is okay, means we never get bonus cards
	}
	m_boardPtr = std::make_unique( initialCards );
      }
    }
    
    
  
  } // ns game 
} // ns threes
