#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <vector>
#include <list>
#include "common.h"


// Computes BB for the given coordinates in O(n) time
int boundingBox(const std::vector<Coordinate>& coords) {
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
double clique_slow(const std::vector<Coordinate>& coords) {
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
int mst_alt(const std::vector<int>& x_coords, const std::vector<int>& y_coords) { // Kept for comparison
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


// Computes the minimum spanning tree length in O(n^2) time, uses lists and deletion instead of vectors
int mst(const std::vector<Coordinate>& coords) { // Turned out to be faster than the previous version
    if (coords.empty()) return 0;

    // Use something based on prim's algorithm to find the minimum spanning tree
    
    size_t size = coords.size();
    std::vector<int> weights(size, std::numeric_limits<int>::max());
    std::list<std::pair<Coordinate, int>> weighted_vertices; // list of pairs (vertex, weight)

    for (size_t i = 1; i < size; ++i) {
        weighted_vertices.emplace_back(coords[i], weights[i]);
    }

    int total_length = 0;
    Coordinate min_vertex = coords[0]; // the first vertex is the starting point

    // Two nested loops, both ranging over the set of vertices, so O(n^2) runtime
    while(!weighted_vertices.empty()) {
        
        // Reduce weights and find the vertex with the smallest distance
        int min_weight = std::numeric_limits<int>::max();
        auto min_it = weighted_vertices.end();

        for (auto it = weighted_vertices.begin(); it != weighted_vertices.end(); ++it) {
            auto& [vertex, weight] = *it;
            
            // Reduce weights
            int distance = abs(vertex.first - min_vertex.first) + abs(vertex.second - min_vertex.second);
            if(weight > distance) weight = distance;
            
            // Select as next min
            if (weight < min_weight) {
                min_weight = weight;
                min_it = it; // update the iterator to the minimum vertex
            }
        }

        total_length += min_weight;

        // Remove the vertex with the minimum weight from the list
        
        min_vertex = min_it->first; // update the minimum vertex
        weighted_vertices.erase(min_it);

    }
    
    return total_length;

}



// Computes an approximate for the minimal steiner tree length in O(n^3) time
int steiner_approx(const std::vector<Coordinate>& coordinates) {
    if (coordinates.empty()) return 0;

    std::list<Coordinate> terminals(coordinates.begin()++, coordinates.end()); //convert vector to list
    std::list<Coordinate> graph_vertices;
    std::list<Edge> graph_edges;
    std::pair<int, int> t = *coordinates.begin();

    while(not terminals.empty()) { // n iterations

        if(graph_edges.empty()) {
            // Add s to the graph along with the single edge
            Coordinate s = *terminals.begin();
            terminals.erase(terminals.begin());
            graph_vertices.emplace_back(s);
            graph_edges.emplace_back(s,t);
            continue;
        }

        // Find closest coord in coordinates to the graph
        Coordinate s;
        auto s_it = terminals.end();
        Edge uw;
        auto uw_it = graph_edges.end();
        int min_dist = std::numeric_limits<int>::max();

        // Traverse list like this to get an iterator to the edge with the minimum distance    
        // The outer loop has n iterations, inner loop has O(n) iterations since in every step of the while loop
        // only add three edges and remove one edge, so at most 2n edges in the graph -> O(n^2) runtime in total
        for (auto it = terminals.begin(); it != terminals.end(); ++it) {
            for(auto e_it = graph_edges.begin(); e_it != graph_edges.end(); ++e_it) {

                Edge e = *e_it;
                Coordinate u = e.first;
                Coordinate w = e.second;

                // Calculate the distance from the terminal to the edge area
                int dist_to_edge = 0;

                // x-coordinate distance
                if (s.first < u.first && s.first < w.first) { // left of the edge
                    dist_to_edge = std::abs(s.first - u.first);
                } 
                else if (s.first > u.first && s.first > w.first) { // right of the edge
                    dist_to_edge = std::abs(s.first - w.first);
                }
                else {
                    dist_to_edge = 0; // s is in the middle of the edge
                }
                // y-coordinate distance
                if (s.second < u.second && s.second < w.second) { // below the edge
                    dist_to_edge += std::abs(s.second - u.second);
                }
                else if (s.second > u.second && s.second > w.second) { // above the edge
                    dist_to_edge += std::abs(s.second - w.second);
                }
                else {
                    dist_to_edge += 0; // s is in the middle of the edge
                }

                if(dist_to_edge < min_dist) {
                    min_dist = dist_to_edge;
                    uw = e;
                    uw_it = e_it;
                    s_it = it;
                    s = *s_it;  
                }
            }
        }
        s = *s_it;
        terminals.erase(s_it); // remove the terminal from the list

        // find edge e={u,w} in graph which minimizes dist(s,shortest path area(u,w))

        Edge e = *uw_it;
        Coordinate u = e.first;
        Coordinate w = e.second;

        graph_edges.erase(uw_it);

        // Determine vertex v
        Coordinate v; // the vertex on the shortest path area from u to w, that is closest to s
        if (s.first < u.first && s.first < w.first) {
            v.first = std::min(u.first, w.first); // s is left of the edge
        }
        else if (s.first > u.first && s.first > w.first) {
            v.first = std::max(u.first, w.first); // s is right of the edge
        }
        else {
            v.first = s.first; // s is in the middle of the edge
        }

        if (s.second < u.second && s.second < w.second) {
            v.second = std::min(u.second, w.second); // s is below the edge
        }
        else if (s.second > u.second && s.second > w.second) {
            v.second = std::max(u.second, w.second); // s is above the edge
        }
        else {
            v.second = s.second; // s is in the middle of the edge
        }

        graph_vertices.emplace_back(v);
        graph_edges.emplace_back(u, v);
        graph_edges.emplace_back(w, v);
        // If s and v differ, add edge (s,v)
        if (s.first != v.first || s.second != v.second) {
            graph_vertices.emplace_back(s);
            graph_edges.emplace_back(s, v);
        }
    }
    int total_distance = 0;
    //TODO add distances for all graph edges
    for (const auto& edge : graph_edges) {
        total_distance += std::abs(edge.first.first - edge.second.first) + std::abs(edge.first.second - edge.second.second);
    }
    return total_distance;
}