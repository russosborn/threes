#include <Card.h>
#include <cmath>

double threes::game::standardCardScore(const threes::game::Card cardData) {
      if( cardData.value > 2) {
	const double numTimesCombined =
	  std::log2(static_cast<double>(cardData.value)/3.0 ) + 1.0;
	return std::pow(3.0, numTimesCombined);
      }
      return 0.0;

}
