#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "common.h"
#include "algorithms.cpp"
// only for testing purposes
#include <chrono>

void timing(const std::vector<Coordinate>& coordinates, const std::vector<int>& x_coordinates, const std::vector<int>& y_coordinates, int algorithm) {
    std::string algorithm_names[] = {
        "Bounding Box",
        "Clique",
        "Clique Slow",
        "Star",
        "MST",
        "MST Alternative",
        "Steiner Approximation"
    };
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10000; ++i) {
        switch (algorithm) {
        case 0: // Bounding Box
            boundingBox(coordinates);
            break;
        case 1: // Clique
            clique(x_coordinates, y_coordinates);
            break;
        case 2: // Clique Slow
            clique_slow(coordinates);
            break;
        case 3: // Star
            star(x_coordinates, y_coordinates);
            break;
        case 4: // MST
            mst(coordinates);
            break;
        case 5: // MST alternative
            mst_alt(x_coordinates, y_coordinates);
            break;
        case 6: // Steiner Approximation
            steiner_approx(coordinates);
            break;
        default:
            std::cerr << "Unknown algorithm" << std::endl;
            return;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time for algorithm " << algorithm_names[algorithm] << ": " << elapsed.count() << " seconds" << std::endl;
}

int main() {
    std::vector<Coordinate> coordinates;

    //TODO: fix reading from input pipe
    //when is the input pipe done? Maybe when std::ends is written?
    /*
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int x, y;
        if (iss >> x >> y) {
            coordinates.emplace_back(x, y);
            std::cout << "Read x=" << x << ", y=" << y << std::endl;
        } else {
            std::cerr << "Invalid input: " << line << std::endl;
        }
    }
    */

    // Example coordinates for testing
    int size = 50; // Number of coordinates
    int range = 50; // Range for random coordinates
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < size; ++i) {
        int x = rand() % range; // Random x-coordinate between 0 and 19
        int y = rand() % range; // Random y-coordinate between 0 and 19
        coordinates.emplace_back(x, y);
    }

    std::vector<int> x_coordinates;
    std::vector<int> y_coordinates;

    for (auto &c : coordinates)
    {
        x_coordinates.emplace_back(c.first);
        y_coordinates.emplace_back(c.second);
    }

    std::cout << boundingBox(coordinates) << std::endl;

    // std::cout << clique_slow(coordinates) << std::endl; // Slower for sufficiently large inputs
    std::cout << clique(x_coordinates, y_coordinates) << std::endl;

    std::cout << star(x_coordinates, y_coordinates) << std::endl;

    std::cout << mst(coordinates) << std::endl;
    // std::cout << mst_alt(x_coordinates, y_coordinates) << std::endl; // Seems like it is always slower than mst

    std::cout << steiner_approx(coordinates) << std::endl;

    
    // Timing analysis
    /*
    timing(coordinates, x_coordinates, y_coordinates, 0); // Bounding Box
    timing(coordinates, x_coordinates, y_coordinates, 1); // Clique
    timing(coordinates, x_coordinates, y_coordinates, 2); // Clique Slow
    timing(coordinates, x_coordinates, y_coordinates, 3); // Star
    timing(coordinates, x_coordinates, y_coordinates, 4); // MST
    timing(coordinates, x_coordinates, y_coordinates, 5); // MST Alternative
    timing(coordinates, x_coordinates, y_coordinates, 6); // Steiner Approximation
    std::cout << "All algorithms executed successfully." << std::endl;
    */
    return 0;
}

