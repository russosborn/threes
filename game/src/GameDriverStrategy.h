#pragma once

#include "Utils.h"
#include "Board.h"
#include "GameDriver.h"
#include <string>
#include <random>
#include <memory>

namespace threes {
  namespace game {


    template<class BOARD>
    class IThreesStgy {
    public:
      virtual ShiftDirection move(const typename GameDriver<BOARD>::BoardPtr& boardPtr,
			      const typename ICardSequence<BOARD>::ICardSeqPtr& seqPtr) = 0;

      using ThreesStgyPtr = std::unique_ptr< IThreesStgy<BOARD> >;
      
    };

    

    template<class BOARD>
    class RandomStgy : public IThreesStgy<BOARD> {
    public:
      virtual ShiftDirection move(const typename GameDriver<BOARD>::BoardPtr& boardPtr,
			      const typename ICardSequence<BOARD>::ICardSeqPtr& seqPtr) override {

	std::random_device rd;
	std::mt19937 g(rd());
	//todo: this assumes contiguous direction ENUM starting at 0
	std::uniform_int_distribution<> dist(0, NUM_DIRECTIONS-1);
	
	return ShiftDirection( dist(g) );
      }

    };
    
////////////////////////////////////////////////////
    
    template<class BOARD>
    class GameDriverStgy : public GameDriver<BOARD> {
    public:
      GameDriverStgy(const std::string& sequencerType,
		     const std::string& sequencerArgs,
		     const unsigned numStartCards,
		     typename IThreesStgy<BOARD>::ThreesStgyPtr& stgyPtr)
	: GameDriver<BOARD>(sequencerType, sequencerArgs, numStartCards)
	, m_stgyPtr(std::move(stgyPtr))
	{}
	  
      virtual uint64_t play() override; // GameDriver interface

    protected:
      virtual void render() const override {} // no render for automated play

    protected:
      using GameDriver<BOARD>::m_boardPtr;
      using GameDriver<BOARD>::m_cardSeqPtr;
      
      typename IThreesStgy<BOARD>::ThreesStgyPtr m_stgyPtr;
      
    }; // class GameDriverStgy


    template<class BOARD>
    uint64_t GameDriverStgy<BOARD>::play() {

      static constexpr unsigned MAX_CONSEC_INVALID = 1000u;
      
      unsigned numConsecInvalid=0;
      MoveResult lastMove=MOVE_INVALID;
      while( lastMove != END_GAME ) {

	ShiftDirection moveDir = m_stgyPtr->move(m_boardPtr, m_cardSeqPtr);

	lastMove = this->move(moveDir);
	if(lastMove != MOVE_INVALID) {
	  numConsecInvalid = 0;
	} else {
	  ++numConsecInvalid;
	  ASSERT(numConsecInvalid < MAX_CONSEC_INVALID,
		 "strategy did many invalid moves in a row, giving up to avoid infinte loop");
	}
	
      }

      uint64_t score = this->gameScore();

      std::cout << "No more valid moves! Game over, your score is "
		<< score << std::endl;
      std::cout << std::endl << std::endl;
      
      return score;
    }
    
    
  } // ns game
} // ns threes

