#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

int main() {
    std::string inputFile = "input.txt";
    std::string outputFile = "output.txt";

    // Read input file
    std::ifstream in(inputFile);
    if (!in) {
        std::cerr << "Error: Could not open " << inputFile << "\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    in.close();

    if (lines.size() < 30) {
        std::cerr << "Error: Need at least 30 lines, but only "
                  << lines.size() << " found.\n";
        return 1;
    }

    // Shuffle lines
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lines.begin(), lines.end(), g);

    // Get top 30
    std::vector<double> thirty;
    for (int i = 0; i < 30; i++) {
        try {
            thirty.push_back(std::stod(lines[i]));
        } catch (...) {
            std::cerr << "Error: Non-numeric line encountered.\n";
            return 1;
        }
    }

    // Sort the 30 numbers
    std::sort(thirty.begin(), thirty.end());

    // Write sorted output
    std::ofstream out(outputFile);
    if (!out) {
        std::cerr << "Error: Could not open " << outputFile << "\n";
        return 1;
    }

    for (double v : thirty) {
        out << v << "\n";
    }

    out.close();

    std::cout << "Done! Wrote and sorted 30 randomized lines to "
              << outputFile << "\n";

    return 0;
}
