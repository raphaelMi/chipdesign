#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <vector>


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

// Computes the clique netlength in O(n^2) time
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

    for (std::size_t i = 1; i < size; i++)
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

    //find median of x and y, O(n log n) without median of medians
    std::vector<int> sorted_x = x_coords;
    std::vector<int> sorted_y = y_coords;
    std::sort(sorted_x.begin(), sorted_x.end()); //O(n log n) operation
    std::sort(sorted_y.begin(), sorted_y.end());
    int median_x = sorted_x[sorted_x.size() / 2];
    int median_y = sorted_y[sorted_y.size() / 2];

    //calculate the sum of distances to the median in linear time
    int total_distance = 0;
    for (size_t i = 0; i < x_coords.size(); ++i) {
        total_distance += std::abs(x_coords[i] - median_x) + std::abs(y_coords[i] - median_y);
    }
    return total_distance;
}

// Computes the minimum spanning tree length in O(n^2) time
int mst(const std::vector<int>& x_coords, const std::vector<int>& y_coords) {
    if (x_coords.empty() or y_coords.empty()) return 0;

    //TODO
    return -1;
}

// Computes an approximate for the minimal steiner tree length in O(n^3) time
int steiner_approx(const std::vector<int>& x_coords, const std::vector<int>& y_coords) {
    if (x_coords.empty() or y_coords.empty()) return 0;

    //TODO
    return -1;
}