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

using Length_vI = std::tuple<Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>;

// To compare lengths 
class lengthComparator 
{ 
  public: 
    bool operator() (const Length_vI& vs1, 
      const Length_vI& vs2) 
    { 
      Coordinate l1 = std::get<0>(vs1);
      Coordinate l2 = std::get<0>(vs2);

      
      if (l1 < 0) return false;  //if first value is infinite
      if (l2 < 0) return true;   //if second value is infinite
      return (l1 < l2); 
    } 
}; 




using LengthHeap = std::priority_queue<Length_vI, 
std::vector<Length_vI>, lengthComparator>;

#endif /*ALGORITHM_H */
