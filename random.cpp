#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

// Terminal colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BLACK "\033[30m"
#define WHITE "\033[37m"
#define BRIGHT_BLACK "\033[90m"  
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

// Terminal styles
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKETHROUGH "\033[9m"

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
        cerr << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "Could not open " << inputFile << RESET << "\n";

        // Ask to create a randomly filled input file if no input file already exists
        while (true) {
            cout << WHITE << BOLD 
                << "Create randomly filled input file? (y/n): "
                << RESET;
            cin >> yn;
            cout << endl;

            if (yn == 'y' || yn == 'n') break;
            
            cout << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
                << "Invalid input! Try again." << RESET << endl;
        }

        if (yn == 'y') {
            createInputFile();
            in.clear();
            in.open(inputFile);
        } else if (yn =='n') {
            cout << BRIGHT_RED << BOLD << "An input file is required. Quitting..." << RESET << " " << endl;
            return 1;   
        } else return 1;

        cin.clear();
        cin.ignore(10000, '\n');
    }

    // Ask the user how many lines to select
    int numLines = 0;
    while (true) {
        cout << WHITE << BOLD 
            << "How many lines do you want to select? (integer >= 0): "
            << RESET;
        if (cin >> numLines && numLines >= 0) {
            cout << endl;
            break;
        }

        cout << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "Invalid input! Try again." << RESET << " " << endl;

        cin.clear();
        cin.ignore(10000, '\n');
    }

    // Ask whether to sort the h->l, l->h, or none (Default = l->h)
    string highLow = "lh";
    while (true) {
        cout << WHITE << BOLD 
            << "How should the output be sorted? (\"hl\"=high->low, \"lh\"=low->high, or \"n\"=none): "
            << RESET;
        cin >> highLow;
        cout << endl;
        if (highLow == "lh" || highLow == "hl" || highLow == "n") break;

        cout << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "Invalid input! Try again." << RESET << " " << endl;

        cin.clear();
        cin.ignore(10000, '\n');
    }

    // Get all of the lines in the file
    vector<string> lines;
    string line;    
    bool INopen = in.is_open();
    while (getline(in, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    in.close();
    INopen = in.is_open();

    if (lines.size() < static_cast<size_t>(numLines)) {
        cerr << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "Need at least " << numLines << " lines, but only " 
            << lines.size() << " found.\n" << RESET << " ";
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
            cerr << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
                << "Non-numeric line encountered.\n" << RESET << " ";
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
        cerr << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "Could not open " << outputFile << RESET << " \n";
        return 1;
    }

    for (double v : topLines) {
        out << v << "\n";
    }

    out.close();

    if (sorted) {
        cout << GREEN << "Done! Wrote and sorted " << numLines << " randomized lines to "
              << outputFile << RESET << " \n";
    } else {
        cout << GREEN << "Done! Wrote " << numLines << " randomized lines to "
              << outputFile << RESET << " \n";
    }

    // Output basic statistics
    double mean = calcMean(topLines), med = calcMedian(topLines), 
        min = calcMin(topLines), max = calcMax(topLines);
    cout << BLUE << "\nCount: " << RESET << CYAN << topLines.size() << RESET << endl;
    cout << BLUE << "Mean: " << RESET << CYAN << mean << RESET << endl;
    cout << BLUE << "Median: " << RESET << CYAN << med << RESET << endl;
    cout << BLUE << "Min: " << RESET << CYAN << min << RESET << endl;
    cout << BLUE << "Max: " << RESET << CYAN << max << RESET << endl;
    calcStDevVar(topLines, mean);

    cout << RESET << endl;
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
    bool open = inFile.is_open();
    if (open) {
        for (int i = 0; i < limit(g); i++) {
            if (i % randMod(g) == 0) {
                inFile << distDoub(g) << "\n";
            } else {
                inFile << distInt(g) << "\n";
            }
        }      
        open = inFile.is_open();
        cout << GREEN << "input.txt file created successfully. " << RESET << " \n";
        return;
    } else {
        cerr << BRIGHT_RED << BOLD << "Error: " << RESET << MAGENTA 
            << "input.txt file not created successfully." << RESET << " \n";
        return;
    }
}

double calcMean(vector<double> nums) {
    double sum = 0, mean = 0;
    int count = 0;
    for (double n : nums) {
        sum += n;
        count++;
    }
    mean = sum / count;
    return mean;
}

double calcMedian(vector<double> nums) {
    // Sort the nums from lowest to highest
    sort(nums.begin(), nums.end());
    
    // Get the count and midpoint
    int count = nums.size();
    int midpoint = count / 2;
    double mid = 0;

    // If the midpoint is an even index have mid be the avg of count/2 and (count/2)-1
    if (midpoint % 2 == 0) {
        mid = (nums[midpoint] + nums[midpoint-1]) / 2;
    } else {
        mid = nums[midpoint];
    }
    
    return mid;
}

void calcStDevVar(vector<double> nums, double mean) {
    double squaredSumDiff = 0;
    for (double n : nums) {
        squaredSumDiff += (n - mean) * (n - mean);
    }

    int count = nums.size();

    double popVar = squaredSumDiff / count;
    double popStDev = sqrt(popVar);
    double sampVar = squaredSumDiff / (count - 1);
    double sampStDev = sqrt(sampVar);

    cout << BLUE << "Population variance: " << RESET << CYAN << popVar << RESET << endl;
    cout << BLUE << "Population standard deviation: " << RESET << CYAN << popStDev << RESET << endl;
    cout << BLUE << "Sample variance: " << RESET << CYAN << sampVar << RESET << endl;
    cout << BLUE << "Sample standard deviation: " << RESET << CYAN << sampStDev << RESET << endl;
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


