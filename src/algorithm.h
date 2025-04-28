#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include "common.h"

namespace Algorithm {
Coordinate dijkstra_steiner(std::vector<Terminal> const& terminals);
}

#endif /*ALGORITHM_H */
