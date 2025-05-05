#include <iostream>
#include "algorithm.h"
#include <queue>
#include <bitset>
#include <vector>
#include <algorithm>

namespace Algorithm {
  
  //Equality comparison for tuples v,I
  bool operator==(const Length_vI& lhs, const Length_vI& rhs) {
    return &lhs == &rhs; // Compare memory addresses
  }
  
  // bool vIinP(Length_vI vI1, std::vector<Length_vI> p)
  // {
  //   for(const auto& vI2 : p)
  //   {
  //     if (vI1 == vI2) return true;
  //   }
  //   return false;
  // }
  
  //returns minimum and maximum x and y coords.
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
  
  //returns BB of a list of vertices
  Coordinate boundingBox(std::vector<Terminal> const& vertices) 
  {
    std::vector<Coordinate> coords = calculateExtremes(vertices);
    return coords[1] - coords[0] + coords[3] - coords[2];
  }
  
  //returns BB of a vertex and a bitset (bitset should be I, not T\I)
  Coordinate fastBoundingBox(Terminal const& t, std::bitset<MAX_NUM_TERMINALS> terminalSet)
  {
    //TODO
    return 0;
  }
  
  void fillHeap(LengthHeap& l, std::vector<Terminal> const& terminals, int n) 
  {
    for (int i=1; i<n; i++)
    {
      std::bitset<MAX_NUM_TERMINALS> s;
      s.set(i);
      Coordinate lb = fastBoundingBox(terminals[i], s);
      l.push_back(Length_vI {0, lb, terminals[i] , s});
    }
    
    //heapify the list
    std::make_heap(l.begin(), l.end(), lengthComparator());
    
    return;
  }
  
  //implements step3 of the dijkstra-steiner algorithm (also step 4 at the end)
  Length_vI step3(LengthHeap& l, std::vector<Length_vI>& p)
  { 
    for(Length_vI lvi : l){
      std::cout << std::get<0>(lvi) << " ";
    }
    std::cout << std::endl;

    //pop the root of the heap
    std::pop_heap(l.begin(), l.end(), lengthComparator());
    Length_vI current_vI = l.back();
    l.pop_back();

    for(Length_vI lvi : l){
      std::cout << std::get<0>(lvi) << " ";
    }
    std::cout << std::endl;
    
    //add it to P
    p.emplace_back(current_vI);
    return current_vI;
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
      Length_vI current_vI = step3(length_heap, smt);

      
      //step 5
      if ((std::get<2>(current_vI) == terminals[0]) &&
      (std::get<3>(current_vI) == allTerminalsWithoutFirst))
      {
        return std::get<0>(current_vI);
      }
      
      //must not actually be a terminal, just a vertex
      Terminal v = std::get<2>(current_vI);
      Coordinate cost = std::get<0>(current_vI);
      std::bitset<MAX_NUM_TERMINALS> terminalSet = std::get<3>(current_vI);  

      std::cout << "step 5 vertex: " << v.x << v.y << " cost: " << cost << std::endl << terminalSet << std::endl;
      
      //step6, first check which edges can be added
      bool addLeft  = v.x != dimensions[0];
      bool addRight = v.x != dimensions[1];
      bool addDown  = v.y != dimensions[2];
      bool addUp    = v.y != dimensions[3];

      //step7, check which (v,J) are viable
      std::vector<Length_vI> list_vJ;
      for(Length_vI p : smt)
      { 	
        Terminal other_v = std::get<2>(p);
        if((v.x == other_v.x)&&( v.y == other_v.y) && (!(std::get<3>(p) & terminalSet).any())){ //if bitsets are disjoint, no bits are true for bitwise AND
          list_vJ.push_back(p);
        }
      }
      std::vector<bool> used_vJ(list_vJ.size(),false);
      
      //iterate through heap to reduce l(v,I) for step 6 and 7, heap property may be violated
      for(Length_vI& vertexTerminals : length_heap) 
      {
        Terminal w = std::get<2>(vertexTerminals);
        std::bitset<MAX_NUM_TERMINALS> terminalsIJ = std::get<3>(vertexTerminals);
        Coordinate oldCost = std::get<0>(vertexTerminals);

        if((v.x == w.x) && (v.y == w.y)) {                      //if v = w we can try to apply step 7
          for(ulong i = 0; i < list_vJ.size(); i++)
          {
            Length_vI vJ = list_vJ[i];
            if((std::get<3>(vJ) | terminalSet) == terminalsIJ) {  //try to match terminal sets to I u J
              Coordinate costJ = std::get<0>(vJ);
              if(cost + costJ < oldCost) oldCost = cost + costJ;  //overwrite old cost
              used_vJ[i] = true;
              break; //break inner for loop
            }
          } continue;
        }
        
        if(!(addLeft || addRight || addDown || addUp)) continue;

        if(addLeft && (w.x +1 == v.x) && (w.y == v.y) && (terminalSet == terminalsIJ))            //try to find vertices on adjacent edges
        {
          if(cost + 1 < oldCost) oldCost = cost+1;
          addLeft = false;
          continue;
        }
        
        if(addRight && (w.x -1 == v.x) && (w.y == v.y)&& (terminalSet == terminalsIJ))
        {
          if(cost + 1 < oldCost) oldCost = cost+1;
          addRight = false;
          continue;
        }
        
        if(addDown && (w.y +1 == v.y) && (w.x == v.x)&& (terminalSet == terminalsIJ))
        {
          
          if(cost + 1 < oldCost) oldCost = cost+1;
          addDown = false;
          continue;
        }
        
        if(addUp && (w.y -1 == v.y) && (w.x == v.x)&& (terminalSet == terminalsIJ))
        {
          if(cost + 1 < oldCost) oldCost = cost+1;
          addUp = false;
          continue;
        }
      }

      for(Length_vI& vertexTerminals : smt) 
      {
        Terminal w = std::get<2>(vertexTerminals);
        std::bitset<MAX_NUM_TERMINALS> terminalsIJ = std::get<3>(vertexTerminals);

        if((v.x == w.x) && (v.y == w.y)) {                      //if v = w we can try to apply step 7
          for(ulong i = 0; i < list_vJ.size(); i++)
          {
            Length_vI vJ = list_vJ[i];
            if((std::get<3>(vJ) | terminalSet) == terminalsIJ) {  //try to match terminal sets to I u J
              used_vJ[i] = true;
              break; //break inner for loop
            }
          } continue;
        }
        
        if(!(addLeft || addRight || addDown || addUp)&& (terminalSet == terminalsIJ)) continue;

        if(addLeft && (w.x +1 == v.x) && (w.y == v.y))            //try to find vertices on adjacent edges
        {
          addLeft = false;
          continue;
        }
        
        if(addRight && (w.x -1 == v.x) && (w.y == v.y)&& (terminalSet == terminalsIJ))
        {
          addRight = false;
          continue;
        }
        
        if(addDown && (w.y +1 == v.y) && (w.x == v.x)&& (terminalSet == terminalsIJ))
        {
          addDown = false;
          continue;
        }
        
        if(addUp && (w.y -1 == v.y) && (w.x == v.x)&& (terminalSet == terminalsIJ))
        {
          addUp = false;
          continue;
        }
      }

      std::make_heap(length_heap.begin(), length_heap.end(), lengthComparator());

      if(addLeft) //if vertex left of v was not found in heap, it's cost was infinite
      {
        std::bitset<MAX_NUM_TERMINALS> s = terminalSet;         //same I
        Terminal w = {v.x-1,v.y};                               //adjust coordinates
        Coordinate lb = fastBoundingBox(w, s);                  //compute lb
        length_heap.push_back(Length_vI {cost + 1, lb, w, s});  //add to heap
        std::push_heap(length_heap.begin(), length_heap.end(), lengthComparator());
        std::cout << "vertex: " << w.x << w.y << " cost: " << cost+1 << std::endl << terminalSet << std::endl;
      }
      if(addRight)
      {
        std::bitset<MAX_NUM_TERMINALS> s = terminalSet;
        Terminal w = {v.x+1,v.y};
        Coordinate lb = fastBoundingBox(w, s);
        length_heap.push_back(Length_vI {cost + 1, lb, w, s});
        std::push_heap(length_heap.begin(), length_heap.end(), lengthComparator());
        std::cout << "vertex: " << w.x << w.y << " cost: " << cost+1 << std::endl << terminalSet << std::endl;
      }
      if(addDown)
      {
        std::bitset<MAX_NUM_TERMINALS> s = terminalSet;
        Terminal w = {v.x,v.y-1};
        Coordinate lb = fastBoundingBox(w, s);
        length_heap.push_back(Length_vI {cost + 1, lb, w, s});
        std::push_heap(length_heap.begin(), length_heap.end(), lengthComparator());
        std::cout << "vertex: " << w.x << w.y << " cost: " << cost+1 << std::endl << terminalSet << std::endl;
      }
      if(addUp)
      {
        std::bitset<MAX_NUM_TERMINALS> s = terminalSet;
        Terminal w = {v.x,v.y+1};
        Coordinate lb = fastBoundingBox(w, s);
        length_heap.push_back(Length_vI {cost + 1, lb, w, s});
        std::push_heap(length_heap.begin(), length_heap.end(), lengthComparator());
        std::cout << "vertex: " << w.x << w.y << " cost: " << cost+1 << std::endl << terminalSet << std::endl;
      }
      
      for(ulong i = 0; i < list_vJ.size(); i++)
      {
        if(used_vJ[i]) continue;
        std::bitset<MAX_NUM_TERMINALS> s = terminalSet | std::get<3>(list_vJ[i]);
        Coordinate l_vIJ = cost + std::get<0>(list_vJ[i]);
        Coordinate lb = fastBoundingBox(v, s);
        length_heap.push_back(Length_vI {l_vIJ, lb, v, s});
        std::push_heap(length_heap.begin(), length_heap.end(), lengthComparator());
        std::cout << "vertex: " << v.x << v.y << " cost: " << l_vIJ << std::endl << s << std::endl;
      }

      std::make_heap(length_heap.begin(), length_heap.end(), lengthComparator());
      
    }
    
    
    
    
    return 0;
  }
  
}
