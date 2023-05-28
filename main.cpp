#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

void parseData(std::ifstream& file, std::vector<std::vector<double>>& data, int expectedColumns) {
  std::string line = "";
  while (std::getline(file, line)) {
    char column = 0;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        if (!token.empty()) {
            //if we got more columns than expected, that's fine. Just add more columns
            //because if we got 20 features, but expected 10, it doesn't matter, we'll just use the first 10
            //but if we got less columns than expected, that's a problem
            try{
                data.at(column).push_back(std::stod(token));
                column++;
            }catch(std::out_of_range& e) {
                data.push_back(std::vector<double>());
                data.at(column).push_back(std::stod(token));
                column++;
            }
        }
    }
    if(column < expectedColumns) {
      throw std::runtime_error("Data contains less columns than expected");
    }
    // std::cout << std::endl;
  }
//   Print data for verification
//   for(size_t i = 0; i < data.size(); i++) {
//     for(size_t j = 0; j < data.at(i).size(); j++) {
//       std::cout << data.at(i).at(j) << " ";
//     }
//     std::cout << std::endl;
//   }
}

bool isFeatureInSet(const std::vector<int>& currentFeatures, const int feature) {
    for(size_t i = 0; i < currentFeatures.size(); i++) {
        if(currentFeatures.at(i) == feature) {
            return true;
        }
    }
    return false;
}


int leaveOneOutCrossValidation(std::vector<std::vector<double>> data, const int featureNum, const int algorithm) {
    // TODO: Implement leave one out cross validation
    return rand() % 101; // Generate a random accuracy between 0 and 100
} 

void forwardSelection(std::vector<std::vector<double>> data) {
    std::vector<int> currentFeatures;
    std::string line = "";
    for(size_t i = 1; i < data.size(); i++){
        std::cout << "On the " << i << "th level of the search tree\n";
        size_t featureToAddAtThisLevel = 0;
        int bestSoFarAccuracy = 0;
        for(size_t j = 1; j < data.size(); j++) {
            if(!isFeatureInSet(currentFeatures, j)) {
                std::cout << "\tConsidering adding the " << j << " feature\n";
                double accuracy = leaveOneOutCrossValidation(data, j, 1);
                if(accuracy > bestSoFarAccuracy) {
                    bestSoFarAccuracy = accuracy;
                    featureToAddAtThisLevel = j;
                }
            }
        }
        currentFeatures.push_back(featureToAddAtThisLevel);
        std::cout << "On level " << i << " I added feature " << featureToAddAtThisLevel << " to current set\n";
    }
}

int main() {
    std::cout << "Welcome to Raymond Yuan, Tangyuan Liang, and Miguelangel Tinoco's Feature Selection Algorithm.\n";

    int featureNum;
    std::cout << "Please enter the total number of features: ";
    std::cin >> featureNum;

    std::vector<std::vector<double>> data(featureNum+1);
    //read in files aand pass data vector to parseFeatures
    std::ifstream file("example.txt");
    if(file.is_open()) {
        try{
            //We should have an extra column for the label, so featureNum+1 columns expected
            parseData(file, data, featureNum+1);
        }
        catch(std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            return -1;
        }
    }
    else {
      std::cout << "Unable to open file" << std::endl;
      return -1;
    }

    int algorithm;
    std::cout << "Type the number of the algorithm you want to run:\n"
              << "\t1) Forward Selection\n"
              << "\t2) Backward Elimination\n"
              << "\t3) Special Algorithm\n";
    std::cin >> algorithm;

    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    forwardSelection(data);
    // int accuracy = 0;
    // accuracy = leaveOneOutCrossValidation(data, featureNum, algorithm);
    // std::cout << "Using no features and 'random' evaluation, I get an accuracy of " << accuracy << "%\n";

    return 0;
}