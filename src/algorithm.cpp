#include <iostream>
#include "algorithm.h"
#include <queue>
#include <bitset>
#include <vector>

namespace Algorithm {

  // To compare lengths 
  class lengthComparator 
  { 
    public: 
      int operator() (const std::tuple<Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>& vs1, 
        const std::tuple<Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>& vs2) 
      { 
        Coordinate l1 = std::get<0>(vs1);
        Coordinate l2 = std::get<0>(vs2);

        
        if (l1 < 0) return false;  //if first value is infinite
        if (l2 < 0) return true;   //if second value is infinite
        return (l1 < l2); 
      } 
  }; 


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



  Coordinate dijkstra_steiner(std::vector<Terminal> const& terminals)
  { 
    //Find out calculateExtremess of Hanan grid
    std::vector<Coordinate> dimensions = calculateExtremes(terminals); 

    const std::size_t numberOfTerminals = terminals.size();
    

    //std::bitset vertexSet;

    std::priority_queue<std::tuple<Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>, 
      std::vector<std::tuple<Coordinate, Terminal, std::bitset<MAX_NUM_TERMINALS>>>, lengthComparator> min_heap;

    

    return static_cast<Coordinate>(terminals.size());
  }

}
