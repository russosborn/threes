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

    

    template<class BOARD>
    void GameDriver<BOARD>::render() const {
      static constexpr unsigned CardValuePrintWidth = 6;
      
      for(unsigned row = 0; row < BOARD::dim; ++row) {
	for(unsigned col = 0; col < BOARD::dim; ++col) {
	  std::cout << std::setw(CardValuePrintWidth) <<
	    m_boardPtr->cardAtIndex( row, col ).value << std::endl;
	}
	std::cout << std::endl << std::endl;
      }
      std::cout << "Next: " << std::setw(CardValuePrintWidth) <<
	m_cardSeqPtr->peek(m_boardPtr).value;
      
    }
    
    template<class BOARD>
    MoveResult GameDriver<BOARD>::move(const ShiftDirection dir) {
      // if shift is valid, then apply it
      if( m_boardPtr->canShift(dir) ) {
	Card toInsert = m_cardSeqPtr->draw()
	  m_boardPtr->shiftBoard(dir, toInsert);
      } else {
	return MOVE_INVALID;
      }

      // check to see if any new move can happen
      bool anyRemainingMoves = false;
      for( auto dir : {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT} ) {
	if( m_boardPtr->canShift(dir) ) {
	  return MOVE_VALID;
	}
      }
      // if no further moves were possible, game should end
      return END_GAME;
    }


    template<class BOARD>
    uint64_t GameDriver<BOARD>::gameScore() {
      // todo: should score counting logic be in the Card class or the
      // GameDriver class? For now, put it here so other GameDriver impls
      // can choose their own method.

      auto boardData = m_boardPtr->underlyingDataRef();
      uint64_t result;
      for( auto cardData : boardData ) {
	// 1 and 2 don't contribute to score
	if( cardData.value > 2) { result += cardData.value; }
      }
      return result;
    }
    

    template<class BOARD>
    uint64_t GameDriver<BOARD>::play() {
      // using w/a/s/d for now because arrow keys are apparently
      // a platform-specific nightmare. Could possible solve with ncurses
      // or a real GUI

      char moveChar = 'x';
      MoveResult lastMove = MOVE_VALID;
      while( lastMove != END_GAME ) {

	if( lastMove == MOVE_INVALID) {
	  std::cout << "You tried move " << moveChar
		    << " but that move is illegal."
		    << "Try a different move. " << std::endl;
	}
	
	std::cin >> moveChar;

	bool validInput = true;
	ShiftDirection moveDir;
	switch(moveChar) {
	case 'w':
	  moveDir = DIRECTION_UP;
	  break;
	case 'a':
	  moveDir = DIRECTION_LEFT;
	  break;
	case 's':
	  moveDir = DIRECTION_DOWN;
	  break;
	case 'd':
	  moveDir = DIRECTION_RIGHT;
	  break;
	default:
	  validInput = false;
	  break;
	}

	if( !validInput ) {
	  std::cout << "HEY! Input one of w/a/s/d for up/left/down/right!"
	}
	else {
	  lastMove = move(moveDir);
	}
      }

      std::cout << "No more valid moves! Game over, your score is "
		<< gameScore() << std::endl;
    }
    
  } // ns game 
} // ns threes
