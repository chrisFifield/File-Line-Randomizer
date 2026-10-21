#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

void createInputFile();
double calcMean(vector<double> nums);
double calcMedian(vector<double> nums);
void calcStDevVar(vector<double> nums, double mean);
double calcMin(vector<double> nums);
double calcMax(vector<double> nums);

int main() {
    string inputFile = "input.txt";
    string outputFile = "output.txt";
    char yn = 'n';

    // Read input file
    ifstream in(inputFile);
    if (!in) {
        cerr << "Error: Could not open " << inputFile << "\n";

        // Ask to create a randomly filled input file if no input file already exists
        while (true) {
            cout << "Create randomly filled input file? (y/n): ";
            cin >> yn;
            cout << endl;

            if (yn == 'y' || yn == 'n') break;
            
            cout << "Error: Invalid input! Try again." << endl;
        }

        if (yn == 'y') {
            createInputFile();
        } else if (yn =='n') {
            cout << "An input file is required. Quitting..." << endl;
            return 1;   
        } else return 1;
    }

    // Ask the user how many lines to select
    int numLines = 0;
    cout << "How many lines do you want to select? (integer >= 0): ";
    cin >> numLines;
    cout << endl;

    // Ask whether to sort the h->l, l->h, or none (Default = l->h)
    string highLow = "lh";
    while (true) {
        cout << "How should the output be sorted? (\"hl\"=high->low, \"lh\"=low->high, or \"n\"=none): ";
        cin >> highLow;
        cout << endl;
        if (highLow == "lh" || highLow == "hl" || highLow == "n") break;

        cout << "Error: Invalid input! Try again." << endl;
    }

    // Get all of the lines in the file
    vector<string> lines;
    string line;
    while (getline(in, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    in.close();

    if (lines.size() < static_cast<size_t>(numLines)) {
        cerr << "Error: Need at least " << numLines << " lines, but only "
                  << lines.size() << " found.\n";
        return 1;
    }

    // Shuffle lines
    random_device rd;
    mt19937 g(rd());
    shuffle(lines.begin(), lines.end(), g);

    // Get top numLines
    vector<double> topLines;
    for (int i = 0; i < numLines; i++) {
        try {
            topLines.push_back(stod(lines[i]));
        } catch (...) {
            cerr << "Error: Non-numeric line encountered.\n";
            return 1;
        }
    }

    // Sort the numLines numbers
    bool sorted = false;
    if (highLow == "lh") {
        sort(topLines.begin(), topLines.end()); // l->h
        sorted = true;
    } else if (highLow == "hl") {
        sort(topLines.begin(), topLines.end(), greater<double>()); // h->l
        sorted = true;
    }
    // if it is not "lh" or "hl" neither branch will run so it wont be sorted at all

    // Write sorted output
    ofstream out(outputFile);
    if (!out) {
        cerr << "Error: Could not open " << outputFile << "\n";
        return 1;
    }

    for (double v : topLines) {
        out << v << "\n";
    }

    out.close();

    if (sorted) {
        cout << "Done! Wrote and sorted " << numLines << " randomized lines to "
              << outputFile << "\n";
    } else {
        cout << "Done! Wrote " << numLines << " randomized lines to "
              << outputFile << "\n";
    }

    // Output basic statistics
    double mean = calcMean(topLines), med = calcMedian(topLines), 
        min = calcMin(topLines), max = calcMax(topLines);
    cout << "\nCount: " << topLines.size() << endl;
    cout << "Mean: " << mean << endl;
    cout << "Median: " << med << endl;
    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;
    calcStDevVar(topLines, mean);

    return 0;
}

void createInputFile() {
    random_device rdNum;
    mt19937 g(rdNum());

    uniform_int_distribution<int> limit(100, 500);
    uniform_int_distribution<int> randMod(2, 10);
    uniform_int_distribution<int> distInt(-500, 500);
    uniform_real_distribution<double> distDoub(-500.00, 500.000);

    ofstream inFile("input.txt");
    if (inFile.is_open()) {
        for (int i = 0; i < limit(g); i++) {
            if (i % randMod(g) == 0) {
                inFile << distDoub(g) << "\n";
            } else {
                inFile << distInt(g) << "\n";
            }
        }

        inFile.close();
        return;
    } else {
        cerr << "Error: input.txt file not created successfully." << "\n";
        return;
    }
}

double calcMean(vector<double> nums) {
    double sum = 0;
    int count = 0;
    for (double n : nums) {
        sum += n;
        count++;
    }
    return sum / count;
}

double calcMedian(vector<double> nums) {
    // Sort the nums from lowest to highest
    sort(nums.begin(), nums.end());
    
    // Get the count and midpoint
    int count = nums.size();
    int midpoint = count / 2;

    // Return the value in the middle of the vector
    return nums[midpoint];
}

void calcStDevVar(vector<double> nums, double mean) {
    double squaredSumDiff = 0;
    for (double n : nums) {
        squaredSumDiff = (n - mean) * (n - mean);
    }

    int count = nums.size();

    double popVar = squaredSumDiff / count;
    double popStDev = sqrt(popVar);
    double sampVar = squaredSumDiff / (count - 1);
    double sampStDev = sqrt(sampVar);

    cout << "Population variance: " << popVar << endl;
    cout << "Population standard deviation: " << popStDev << endl;
    cout << "Sample variance: " << sampVar << endl;
    cout << "Sample standard deviation: " << sampStDev << endl;
}

double calcMin(vector<double> nums) {    
    // Sort the nums from lowest to highest
    sort(nums.begin(), nums.end());
    return nums[0]; // return lowest
}

double calcMax(vector<double> nums) {
    // Sort the nums from highest to lowest
    sort(nums.begin(), nums.end(), greater<double>());
    return nums[0]; // return highest
}


