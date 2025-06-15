#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <vector>
#include <list>


// Computes BB for the given coordinates in O(n) time
int boundingBox(const std::vector<std::pair<int, int>>& coords) {
    if (coords.empty()) return 0;

    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const auto& coord : coords) {
        minX = std::min(minX, coord.first);
        maxX = std::max(maxX, coord.first);
        minY = std::min(minY, coord.second);
        maxY = std::max(maxY, coord.second);
    }

    return maxX - minX + maxY - minY;
}

// Computes the clique netlength in O(n^2) time (kept just for comparison)
double clique_slow(const std::vector<std::pair<int, int>>& coords) {
    if (coords.empty()) return 0;

    // Calculate the clique netlength 
    int total_length = 0;
    for (size_t i = 0; i < coords.size(); ++i) { //runtime O(n^2)
        for (size_t j = i + 1; j < coords.size(); ++j) {
            int dx = coords[i].first - coords[j].first;
            int dy = coords[i].second - coords[j].second;
            total_length += std::abs(dx) + std::abs(dy);
        }
    }


    return static_cast<double>(total_length) / static_cast<double>(coords.size() - 1);
}


// More complex algorithm with O(n log n) complexity
double clique(const std::vector<int>& x_coords, const std::vector<int>& y_coords) {
    if (x_coords.empty() or y_coords.empty()) return 0;

    std::size_t size = x_coords.size();

    // Calculate the clique netlength 
    int total_length = 0;

    std::vector<int> sorted_x = x_coords;
    std::vector<int> sorted_y = y_coords;
    std::sort(sorted_x.begin(), sorted_x.end()); //O(n log n)
    std::sort(sorted_y.begin(), sorted_y.end());

    for (std::size_t i = 1; i < size; ++i)
    {
        // Calculate distances of segments, 
        // then multiply by points on the left and points on the right.
        // This accounts for how many times each segment is used
        total_length += (sorted_x[i] - sorted_x[i-1]) * (i * (size - i));
        total_length += (sorted_y[i] - sorted_y[i-1]) * (i * (size - i));
    }

    return static_cast<double>(total_length) / static_cast<double>(size - 1);
}

// Computes star netlength in O(n log n) time
int star(const std::vector<int>& x_coords, const std::vector<int>& y_coords) {
    if (x_coords.empty() or y_coords.empty()) return 0;
    // Calculates the l1 distance to the median of x and y coordinates

    // Find median of x and y, O(n log n) without median of medians
    std::vector<int> sorted_x = x_coords;
    std::vector<int> sorted_y = y_coords;
    std::sort(sorted_x.begin(), sorted_x.end()); //O(n log n) operation
    std::sort(sorted_y.begin(), sorted_y.end());
    int median_x = sorted_x[sorted_x.size() / 2];
    int median_y = sorted_y[sorted_y.size() / 2];

    // Calculate the sum of distances to the median in linear time
    int total_distance = 0;
    for (size_t i = 0; i < x_coords.size(); ++i) {
        total_distance += std::abs(x_coords[i] - median_x) + std::abs(y_coords[i] - median_y);
    }
    return total_distance;
}

// Computes the minimum spanning tree length in O(n^2) time
int mst(const std::vector<int>& x_coords, const std::vector<int>& y_coords) {
    if (x_coords.empty() or y_coords.empty()) return 0;

    // Use something based on prim's algorithm to find the minimum spanning tree
    
    size_t size = x_coords.size();
    std::vector<int> weights(size, std::numeric_limits<int>::max());
    std::vector<bool> in_tree(size, false);

    int total_length = 0;
    size_t min_index = 0;

    // Two nested loops, both ranging over the set of vertices, so O(n^2) runtime
    for (size_t i = 0; i < size - 1; ++i) {
        
        in_tree[min_index] = true;
        
        // Reduce weights and find the vertex with the smallest distance
        int next_min_weight = std::numeric_limits<int>::max();
        size_t next_min_index = 0;

        for (size_t j = 1; j < size; ++j) {
            if (in_tree[j]) continue;

            // Reduce weights
            int weight = abs(x_coords[j] - x_coords[min_index]) + abs(y_coords[j] - y_coords[min_index]);
            if(weights[j] > weight) weights[j] = weight;
            
            // Select as next min
            if (weights[j] < next_min_weight) {
                next_min_weight = weights[j];
                next_min_index = j;
            }
        }

        min_index = next_min_index;
        total_length += next_min_weight;

    }
    
    return total_length;

}

// Computes an approximate for the minimal steiner tree length in O(n^3) time
int steiner_approx(const std::vector<std::pair<int, int>>& coordinates) {
    if (coordinates.empty()) return 0;

    std::list<std::pair<int, int>> terminals(coordinates.begin(), coordinates.end()); //convert vector to list
    std::list<std::pair<int, int>> graph_vertices;
    std::list<std::pair<size_t,size_t>> graph_edges; //incidence matrix for edges
    //std::pair<int, int> t = terminals.back();
    terminals.pop_back();

    while(not terminals.empty()) {
        //find closest coord in coordinates to the graph
        std::pair<int, int> s;
        for (size_t i = 0; i < terminals.size(); i++) {
            break; //TODO
        }
        auto it = std::find(terminals.begin(), terminals.end(), s);
        if (it != terminals.end()) {
            terminals.erase(it);
        }

        if(graph_edges.empty()) {
            //add s to the graph along with the single edge
            graph_vertices.emplace_back(s);
            graph_edges.emplace_back(std::pair{0, 1});
        } else {
            std::pair<size_t, size_t> e;
            std::pair<int, int> v;
            //find edge {u,w} in graph which minimizes dist(s,shortest path area(u,w))
            
            //graph_edges.erase(e); //same as before
            graph_vertices.emplace_back(s);
            graph_vertices.emplace_back(v);
            graph_edges.emplace_back(std::pair{/*u,v*/ 0,1}); //TODO add logic
            graph_edges.emplace_back(std::pair{/*v,w*/ 0,1});
            graph_edges.emplace_back(std::pair{/*v,s*/ 0,1});

        }

    }
    //int total_distance = 0;
    //TODO add distances for all graph edges
    
    return -1;
}