#include <iostream>
#include "algorithm.h"

namespace Algorithm {

Coordinate dijkstra_steiner(std::vector<Terminal> const& terminals)
{
  std::cerr << "Algorithm not implemented, returning number of terminals" << std::endl;
  return static_cast<Coordinate>(terminals.size());
}

}
