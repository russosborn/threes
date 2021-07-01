#pragma once

#include "Board.h"
#include "Card.h"
#include "CardSequence.h"
#include "Utils.h"

#include <iostream>
#include <iomanip>
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
      using CardSequencePtr =  typename ICardSequence<BOARD>::ICardSeqPtr;
      
    public:
      GameDriver(const std::string& sequencerType,
		 const std::string& sequencerArgs,
		 const unsigned numStartCards);

      virtual uint64_t play() = 0; // returns the final score

    public:
      static constexpr unsigned StateSize = BOARD::StateSize; 
      
    protected:
      virtual void render() const = 0;

      MoveResult move(const ShiftDirection dir); 

      uint64_t gameScore() const;

      
    protected:
      BoardPtr m_boardPtr;
      CardSequencePtr m_cardSeqPtr;
      
    }; // class GameDriver


    // specific impl using std::istream
    // use std::cin for real play, some other stream for testing.
    template<class BOARD>
    class GameDriverIStream : public GameDriver<BOARD> {
    public:
      GameDriverIStream(const std::string& sequencerType,
			const std::string& sequencerArgs,
			const unsigned numStartCards,
			std::istream& inputStream)
	: GameDriver<BOARD>(sequencerType, sequencerArgs, numStartCards)
	, m_inputStream(inputStream)
	{}
      
      virtual uint64_t play() override;
      
    protected:
      virtual void render() const override;


    protected:
      std::istream& m_inputStream;

      using GameDriver<BOARD>::m_boardPtr;
      using GameDriver<BOARD>::m_cardSeqPtr;
      
    }; // class GameDriverIStream
    
    /////////////////////////////////////
    template<class BOARD>
    GameDriver<BOARD>::GameDriver(const std::string& sequencerType,
				  const std::string& sequencerArgs,
				  const unsigned numStartCards)
      : m_cardSeqPtr(ICardSequence<BOARD>::s_factory.create(sequencerType, sequencerArgs))
      {
	std::vector<Card> initialCards(numStartCards);
	for(auto itr = initialCards.begin(); itr != initialCards.end(); ++itr) {
	  *itr = m_cardSeqPtr->draw(nullptr); // null board is okay, means we never get bonus cards
	}
	m_boardPtr = std::make_unique<BOARD>( initialCards );
      }

    

    template<class BOARD>
    void GameDriverIStream<BOARD>::render() const {
      static constexpr unsigned CardValuePrintWidth = 6;
      
      for(unsigned row = 0; row < BOARD::dim; ++row) {
	for(unsigned col = 0; col < BOARD::dim; ++col) {
	  std::cout << std::setw(CardValuePrintWidth) <<
	    m_boardPtr->cardAtIndex( row, col ).value;
	}
	std::cout << std::endl << std::endl;
      }
      std::cout << "Next: " << std::setw(CardValuePrintWidth) <<
	m_cardSeqPtr->peek(m_boardPtr).value << std::endl;
      
    }
    
    template<class BOARD>
    MoveResult GameDriver<BOARD>::move(const ShiftDirection dir) {
      // if shift is valid, then apply it
      if( m_boardPtr->canShift(dir) ) {
	Card toInsert = m_cardSeqPtr->draw(m_boardPtr);
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
    uint64_t GameDriver<BOARD>::gameScore() const {
      static constexpr double EPSILON = 1e-2;
      // todo: should score counting logic be in the Card class or the
      // GameDriver class? For now, put it here so other GameDriver impls
      // can choose their own method.

      auto boardData = m_boardPtr->underlyingDataRef();
      uint64_t result=0;
      for( auto cardData : boardData ) {
	// 1 and 2 don't contribute to score
	if( cardData.value > 2) {
	  const double numTimesCombined =
	    std::log2(static_cast<double>(cardData.value)/3.0 ) + 1.0;
	  const double currCardScoreDbl = std::pow(3.0, numTimesCombined);

	  result += static_cast<uint64_t>(currCardScoreDbl + EPSILON);
	}
      }
      return result;
    }
    

    template<class BOARD>
    uint64_t GameDriverIStream<BOARD>::play() {
      // using w/a/s/d for now because arrow keys are apparently
      // a platform-specific nightmare. Could possible solve with ncurses
      // or a real GUI

      char moveChar = 'x';
      MoveResult lastMove = MOVE_VALID;
      while( lastMove != END_GAME ) {

	this->render();
	
	if( lastMove == MOVE_INVALID) {
	  std::cout << "You tried move " << moveChar
		    << " but that move is illegal."
		    << "Try a different move. " << std::endl;
	}
	
	m_inputStream >> moveChar;

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
	  std::cout << "HEY! Input one of w/a/s/d for up/left/down/right!" << std::endl;
	}
	else {
	  lastMove = this->move(moveDir);
	}
      }

      uint64_t score = this->gameScore();
      std::cout << "******************************" << std::endl;
      std::cout << "******************************" << std::endl;
      std::cout << "******************************" << std::endl;

      std::cout << "No more valid moves! Game over, your score is "
		<< score << std::endl;
      std::cout << std::endl << std::endl;

      this->render();

      return score;
    }
    
  } // ns game 
} // ns threes
