#include <iostream>
#include "algorithm.h"
#include <queue>
#include <bitset>
#include <vector>

namespace Algorithm {

  //Equality comparison for tuples v,I
  bool operator==(const Length_vI& lhs, const Length_vI& rhs) {
    return &lhs == &rhs; // Compare memory addresses
  }

  bool vIinP(Length_vI vI1, std::vector<Length_vI> p)
  {
    for(const auto& vI2 : p)
    {
      if (vI1 == vI2) return true;
    }
    return false;
  }

  std::vector<Coordinate> calculateExtremes(std::vector<Terminal> const& vertices)
  { 
    Coordinate minX = vertices[0].x, maxX = vertices[0].x;
    Coordinate minY = vertices[0].y, maxY = vertices[0].y;
    for (const auto& vertex : vertices) {
      if (vertex.x < minX) minX = vertex.x;
      if (vertex.x > maxX) maxX = vertex.x;
      if (vertex.y < minY) minY = vertex.y;
      if (vertex.y > maxY) maxY = vertex.y;
  }
      return {minX, maxX, minY, maxY};
  }

  Coordinate boundingBox(std::vector<Terminal> const& vertices) 
  {
    std::vector<Coordinate> coords = calculateExtremes(vertices);
    return coords[1] - coords[0] + coords[3] - coords[2];
  }

  void fillHeap(LengthHeap& l, std::vector<Terminal> const& terminals, int n) 
  {
    for (int i=1; i<n; i++)
    {
      std::bitset<MAX_NUM_TERMINALS> s;
      s.set(i);
      l.push(Length_vI {0, terminals[i] , s});
    }
    return;
  }

  //implements step3 of the dijkstra-steiner algorithm (also step 4 at the end)
  Length_vI step3(LengthHeap l, std::vector<Terminal> const& terminals, std::vector<Length_vI>& p)
  {
    bool reachedMin = false;
    Coordinate minWithBound = 2^31-1;
    Length_vI min_vI;
    
    LengthHeap copy = l;

    do {
      if(copy.empty()) break;

      Length_vI current_vI = copy.top();
      copy.pop();

      if (vIinP(current_vI, p)) continue;

      Coordinate currentPlus_lb = std::get<0>(current_vI) + 0; //TODO add feasible lower bound

      if (minWithBound > currentPlus_lb){ 
        minWithBound = currentPlus_lb;
        min_vI = current_vI;
        continue;
      } 

      if(std::get<0>(current_vI) > minWithBound) reachedMin = true;

    } while(!reachedMin);

    p.emplace_back(min_vI);
    return min_vI;
  }

  Coordinate dijkstra_steiner(std::vector<Terminal> const& terminals)
  { 
    //Find out minX,maxX,minY,maxY of Hanan grid
    std::vector<Coordinate> dimensions = calculateExtremes(terminals); 

    const int numberOfTerminals = terminals.size();
    std::bitset<MAX_NUM_TERMINALS> allTerminalsWithoutFirst;              //example of bit operations below
    allTerminalsWithoutFirst.set();                                       //1111111111111111111111
    allTerminalsWithoutFirst >>= MAX_NUM_TERMINALS - numberOfTerminals;   //0000000011111111111111
    allTerminalsWithoutFirst.reset(0);                                    //0000000011111111111110


    //define l(v,I) data structure
    LengthHeap length_heap;
    fillHeap(length_heap, terminals, numberOfTerminals);

    //define set P of sets l(v,I) = smt({v} u I)
    std::vector<Length_vI> smt;

    while(true) {
      Length_vI current_vI = step3(length_heap, terminals, smt);

      //step 5
      if ((std::get<1>(current_vI) == terminals[0]) &&
          (std::get<2>(current_vI) == allTerminalsWithoutFirst))
      {
        return std::get<0>(current_vI);
      }

      
    }

    
    



    return 0;
  }

}
