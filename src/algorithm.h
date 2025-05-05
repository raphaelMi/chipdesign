#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include "common.h"
#include <queue>
#include <bitset>
#include <vector>

namespace Algorithm {
Coordinate dijkstra_steiner(std::vector<Terminal> const& terminals);
}

//for each (v,I) save (l(v,I), lb(v,T\I), v , I)
using Length_vI = std::tuple<Coordinate, Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>;

// To compare lengths 
class lengthComparator 
{ 
  public: 
    bool operator() (Length_vI const& vs1, 
      Length_vI const& vs2) 
    { 
      //compare l(v,I)+lb(v,T\I)
      Coordinate l1 = std::get<0>(vs1)+std::get<1>(vs1);
      Coordinate l2 = std::get<0>(vs2)+std::get<1>(vs2);

      return (l1 > l2); 
    } 
}; 




using LengthHeap = std::vector<Length_vI>;

#endif /*ALGORITHM_H */
