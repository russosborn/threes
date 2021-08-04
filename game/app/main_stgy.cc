#include <src/GameDriverStrategy.h>
#include <src/TreeStrategy.h>
#include <src/Board.h>

#include <string>

template<typename BOARD>
void registerCreators() {
  threes::game::ICardSequence<BOARD>::s_factory.registerCreator(
    "k28d",
    threes::game::Kamikaze28Sequence<BOARD>::create);

  threes::game::IThreesStgy<BOARD>::s_factory.registerCreator(
    "random",
    threes::game::RandomStgy<BOARD>::create);

  threes::game::IThreesStgy<BOARD>::s_factory.registerCreator(
    "emtree",
    threes::game::ExpectiMaxTree<BOARD>::create);
}

int main(int argc, char** argv) {

  unsigned repeats=1;
  std::string stgyName("random");
  std::string stgyArgs("");
  if(argc > 1) { repeats = std::stoi(argv[1]); }
  if(argc > 2) { stgyName = argv[2]; }
  if(argc > 3) { stgyArgs = argv[3]; }

  std::cout << "Running strategy " << stgyName << " with args " << stgyArgs << std::endl;

  using ProdBoard = threes::game::Board<4>;
  registerCreators<ProdBoard>();

  for(unsigned i = 0; i < repeats; ++i) {
    threes::game::IThreesStgy<ProdBoard>::ThreesStgyPtr
      stgyPtr(threes::game::IThreesStgy<ProdBoard>::s_factory.create(stgyName, stgyArgs) );
  
    std::unique_ptr<threes::game::GameDriver<ProdBoard>> game(
      new threes::game::GameDriverStgy<ProdBoard>("k28d", "default", 9, stgyPtr) );
      
    game->play();
  }

}
