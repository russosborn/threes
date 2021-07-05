#include <src/GameDriverStrategy.h>
#include <src/Board.h>


int main() {

  using ProdBoard = threes::game::Board<4>;
  
  threes::game::ICardSequence<ProdBoard>::s_factory.registerCreator(
    "k28d",
    threes::game::Kamikaze28Sequence<ProdBoard>::create);

  threes::game::IThreesStgy<ProdBoard>::ThreesStgyPtr stgyPtr(
    new threes::game::RandomStgy<ProdBoard>() );
  
  std::unique_ptr<threes::game::GameDriver<ProdBoard>> game(
    new threes::game::GameDriverStgy<ProdBoard>("k28d", "default", 9, stgyPtr) );

  game->play();
}
