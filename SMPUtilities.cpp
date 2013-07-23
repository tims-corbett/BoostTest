#include <boost/random.hpp>

#include "SMPUtilities.h"


double SMPUtilities::rand01()
{
  boost::mt19937 rng;
  static boost::uniform_01<boost::mt19937> zeroone(rng);
  double p = zeroone();
  return p;
}
