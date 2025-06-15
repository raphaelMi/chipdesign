#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "algorithms.cpp"

int main() {
    std::vector<std::pair<int, int>> coordinates;

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


    coordinates.emplace_back(1, 2);
    coordinates.emplace_back(3, 4);
    coordinates.emplace_back(5, 6);
    coordinates.emplace_back(0, 3);
    coordinates.emplace_back(5, 2);
    coordinates.emplace_back(2, 1);
    coordinates.emplace_back(10, 0);


    std::vector<int> x_coordinates;
    std::vector<int> y_coordinates;

    for (auto &c : coordinates)
    {
        x_coordinates.emplace_back(c.first);
        y_coordinates.emplace_back(c.second);
    }

    std::cout << boundingBox(coordinates) << std::endl;
    std::cout << clique_slow(coordinates) << std::endl;
    std::cout << clique(x_coordinates, y_coordinates) << std::endl;
    std::cout << star(x_coordinates, y_coordinates) << std::endl;
    std::cout << mst(x_coordinates, y_coordinates) << std::endl;
    //std::cout << steiner_approx(coordinates) << std::endl;

    
    return 0;
}

