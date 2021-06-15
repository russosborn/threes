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
      using std::unique_ptr< BOARD > BoardPtr;
      using std::unique_ptr< ICardSqeuence<BOARD> > CardSequencePtr;
      
    public:
      GameDriver(BoardPtr boardPtr,
		 CardSequencePtr sequencePtr,
		 const unsigned numStartCards );

      void play();
      
    protected:
      virtual void render() const;

      MoveResult move(const ShiftDirection dir); 

      uint64_t gameScore() const;

      
    private:
      BoardPtr m_boardPtr;
      CardSequencePtr m_cardSeqPtr;

    }; // class GameDriver


    /////////////////////////////////////
    template<unsigned DIM>
     GameDriver<DIM, SEQUENCER>::GameDriver(const std::string& sequencerType,
					    const unsigned numStartCards)
      : m_cardSeqPtr() // TODO: card sequence factory thingy (string -> args, etc)
      {
	
      }
      
    }
    
    
  
  } // ns game 
} // ns threes
