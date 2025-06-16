#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "common.h"
#include "algorithms.cpp"
// only for testing purposes
#include <chrono>

void timing(const std::vector<Coordinate>& coordinates, 
    const std::vector<int>& x_coordinates, const std::vector<int>& y_coordinates, 
    int algorithm, int iterations) {
    std::string algorithm_names[] = {
        "Bounding Box",
        "Clique O(n log n)",
        "Clique O(n^2)",
        "Star",
        "MST (lists and deletion)",
        "MST alt (vectors)",
        "Steiner Approximation"
    };

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
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

int main(int argc, char* argv[]) {
    bool read_from_input = true; // Default value
    int size = 50; // Default number of coordinates
    int range = 50; // Default range for random coordinates
    bool timing_enabled = false; // Default value for timing
    int iterations; // Number of iterations for timing
    long seed = time(0); // Default seed for random number generation

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no_input") {
            read_from_input = false;
        } else if (arg == "--size" && i + 1 < argc) {
            size = std::stoi(argv[++i]);
        } else if (arg == "--range" && i + 1 < argc) {
            range = std::stoi(argv[++i]);
        } else if (arg == "--seed") {
            seed = (std::stol(argv[++i]));
        } else if (arg == "--timing") {
            timing_enabled = true;
            if (i + 1 < argc) {
                try {
                    iterations = std::stoi(argv[i+1]);
                    ++i;
                } catch(const std::exception& e) { iterations = 10000; }
                std::cout << "Timing enabled with " << iterations << " iterations." << std::endl;
            }
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --no_input       Generate coordinates instead of reading from input\n"
                      << "  --size <n>       Number of coordinates to generate (default: 50)\n"
                      << "  --range <r>      Range for random coordinates (default: 50)\n"
                      << "  --seed <s>       Seed for random generation\n"
                      << "  --timing [m]     Enable timing analysis with m (optional) iterations\n"
                      << "  --help, -h       Show this help message\n";
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    std::vector<Coordinate> coordinates;

    if (read_from_input) {
        std::string line;
        while (std::getline(std::cin, line)) {
            std::istringstream iss(line);
            int x, y;
            if (iss >> x >> y) {
                coordinates.emplace_back(x, y);
            } else {
                std::cerr << "Invalid input: " << line << std::endl;
            }
        }
    } else {
        unsigned s = (unsigned)(seed);
        srand(s);
        for (int i = 0; i < size; ++i) {
            int x = rand() % range; // Random x-coordinate in range
            int y = rand() % range; // Random y-coordinate in range
            // std::cout << "Generated coordinate: (" << x << ", " << y << ")" << std::endl;
            coordinates.emplace_back(x, y);
        }
    }

    std::vector<int> x_coordinates;
    std::vector<int> y_coordinates;

    for (auto &c : coordinates) {
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
    if(timing_enabled) {
        timing(coordinates, x_coordinates, y_coordinates, 0, iterations); // Bounding Box
        timing(coordinates, x_coordinates, y_coordinates, 1, iterations); // Clique
        timing(coordinates, x_coordinates, y_coordinates, 2, iterations); // Clique Slow
        timing(coordinates, x_coordinates, y_coordinates, 3, iterations); // Star
        timing(coordinates, x_coordinates, y_coordinates, 4, iterations); // MST
        timing(coordinates, x_coordinates, y_coordinates, 5, iterations); // MST Alternative
        timing(coordinates, x_coordinates, y_coordinates, 6, iterations); // Steiner Approximation
    }
    return 0;
}

