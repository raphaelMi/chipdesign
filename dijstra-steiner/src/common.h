#ifndef COMMON_H
#define COMMON_H

auto constexpr MAX_NUM_TERMINALS = 20;
using Coordinate = int;

struct Terminal
{
  Coordinate x;
  Coordinate y;
  bool operator==(const Terminal& other) {
    return (x == other.x) && (y == other.y); 
  }
};

#endif /*COMMON_H */
