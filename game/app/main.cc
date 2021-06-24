#include <src/GameDriver.h>
#include <src/Board.h>


int main() {

  using ProdBoard = threes::game::Board<4>;
  
  threes::game::ICardSequence<ProdBoard>::s_factory.registerCreator(
    "k28d",
    threes::game::Kamikaze28Sequence<ProdBoard>::create);
    
  std::unique_ptr<threes::game::GameDriver<ProdBoard>> game(
    new threes::game::GameDriverIStream<ProdBoard>("k28d", "default", 9, std::cin) );

  game->play();
}
