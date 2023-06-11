#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <math.h>
#include <algorithm>

/****************NOTES ABOUT TESTING****************
 * For small-test-dataset
 *     forward selection should return {3, 5} as the best feature subset
 *     backward elimination should return {2, 4, 5, 7, 10} as the best feature subset
 * For large-test-dataset
 *     forward selection should return {1, 27} as the best feature subset
 * When using testing code
 *    leaveOneOutCrossValidation should return 89% for {3, 5, 7} and 94.9% for {1, 15, 27}
***************************************************/

/****************TODO LIST****************
 * Implement special algorithm
 * Implement Z-score normalization, documentation can be found here: https://www.statology.org/z-score-normalization/
 *      As stated in "Project 2 - Part II-4.pdf", we MUST normalize our data!!
 *      Maybe implement a separate normalize(data) function to normalize the data before we do anything else?
 * After implementing Z-score normalization, test if the program still produces the same results as stated above
 * Start doing the experiments using our own unique dataset
 * Implement data visualization, probably using Matplot++ (https://github.com/alandefreitas/matplotplusplus)
 * Write the report
*******************************************/

enum operation {add, del};

void parseData(std::ifstream& file, std::vector<std::vector<double>>& data, const int& expectedColumns);
bool isFeatureInSet(const std::vector<int>& currentFeatures, const int& feature);
double leaveOneOutCrossValidation(const std::vector<std::vector<double>>& data, std::vector<int>& currentSet, const int& feature, const operation& op);
void forwardSelection(std::vector<std::vector<double>> data, const int featureNum);
void randomEval(const std::vector<std::vector<double>> data);
void backwardSelection(std::vector<std::vector<double>> data, const int featureNum);

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input file>\n";
        return -1;
    }

    std::cout << "Welcome to Raymond Yuan, Tangyuan Liang, and Miguelangel Tinoco's Feature Selection Algorithm.\n\n";

    int featureNum;
    std::cout << "Please enter the total number of features: ";
    std::cin >> featureNum;

    std::vector<std::vector<double>> data(featureNum+1);
    //read in files aand pass data vector to parseFeatures
    std::ifstream file(argv[1]);
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
    std::cout << "\nType the number of the algorithm you want to run:\n"
              << "\t1) Forward Selection\n"
              << "\t2) Backward Elimination\n"
              << "\t3) Special Algorithm\n";
    std::cin >> algorithm;
    std::cout << std::endl;

    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    // std::cout << leaveOneOutCrossValidation(data, {3,5}, 7) << std::endl;
    
    //===================== Actual Main Code =====================//
    randomEval(data);
    switch(algorithm) {
        case 1:
            try{
                forwardSelection(data, featureNum);
            }catch(std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                return -1;
            }
            break;
        case 2:
            try{
                backwardSelection(data, featureNum);
            }catch(std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                return -1;
            }
            break;
        case 3:
            // specialAlgorithm(data, featureNum);
            break;
        default:
            std::cout << "Invalid algorithm number!\n";
            return -1;
    }
    //=================================================================//


    //========================Testing Code============================//
    // std::vector<int> currentFeatures = {3, 5};
    // std::cout << "Using features {3, 5, 7} accuracy is:\n";
    // std::cout << leaveOneOutCrossValidation(data, currentFeatures, 7, add) << "%\n";
    // std::cout << "The correct answer is 89%\n";
    //=================================================================//
    // std::vector<int> currentFeatures = {1, 15};
    // std::cout << "Using features {1, 15, 27} accuracy is:\n";
    // std::cout << leaveOneOutCrossValidation(data, currentFeatures, 27, add) << "%\n";
    // std::cout << "The correct answer is 94.9%\n";
    //=================================================================//

    return 0;
}

void parseData(std::ifstream& file, std::vector<std::vector<double>>& data, const int& expectedColumns) {
  std::string line = "";
  while (std::getline(file, line)) {
    char column = 0;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        if (!token.empty()) {
            //if we got more columns than expected, that's fine. Just add more columns
            //because if we got 20 features, but expected 10, it doesn't matter, we'll just use the first 10
            //but if we got 5 features, but expected 10, then we have a problem
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
//       std::cout << data.at(i).at(j) << "\t";
//     }
//     std::cout << std::endl;
//   }
}

bool isFeatureInSet(const std::vector<int>& currentFeatures, const int& feature) {
    for(size_t i = 0; i < currentFeatures.size(); i++) {
        if(currentFeatures.at(i) == feature) {
            return true;
        }
    }
    return false;
}


double leaveOneOutCrossValidation(const std::vector<std::vector<double>>& data, std::vector<int>& currentSet, const int& feature, const operation& op) {
    // return rand() % 101; // Generate a random accuracy between 0 and 100
    if(op == add) {
        currentSet.push_back(feature);
    }
    if(op == del) {
        auto it = std::find(currentSet.begin(), currentSet.end(), feature);
        if(it != currentSet.end()) {
            currentSet.erase(it);
        }else{
            throw std::runtime_error("leaveOneOutCrossValidation: Feature not found in current set");
        }
    }
    unsigned correct = 0;
    for(size_t i = 0; i < data.at(0).size(); i++) {
        char label = data.at(0).at(i); //column 0 is the label
        double distance = std::numeric_limits<double>::max();
        unsigned nearestNeighbor = std::numeric_limits<unsigned>::max();
        //iterate through all the records (rows)
        for(size_t j = 0; j < data.at(0).size(); j++) {
            //if the record is not itself, calculate the distance
            if(i != j) {
                double tempDistance = 0;
                for(size_t k = 0; k < currentSet.size(); k++) {
                    //we're comparing currentSet.at(k)-th column of i-th record with currentSet.at(k)-th column of j-th record
                    tempDistance += std::pow(data.at(currentSet.at(k)).at(i) - data.at(currentSet.at(k)).at(j), 2);
                }
                tempDistance = std::sqrt(tempDistance);
                if(tempDistance < distance) {
                    distance = tempDistance;
                    nearestNeighbor = j;
                }
            }
        }
        if(data.at(0).at(nearestNeighbor) == label) {
            correct++;
        }
    }
    // std::cout << "\t\tAccuracy: " << static_cast<double>(correct)/data.at(0).size() << std::endl;
    return static_cast<double>(correct)/data.at(0).size()*100;
} 

void forwardSelection(std::vector<std::vector<double>> data, const int featureNum) {
    std::vector<int> currentFeatures;
    std::string line = "";
    double bestAccuracy = 0;
    std::vector<std::vector<int>> bestFeatures;
    std::vector<int> bestFeaturesAccuracy;
    std::cout << "Beginning search.\n\n";
    for(size_t i = 1; i < featureNum+1; i++){
        // std::cout << "On the " << i << "th level of the search tree\n";
        size_t featureToAddAtThisLevel = 0;
        double bestSoFarAccuracy = 0;
        for(size_t j = 1; j < featureNum+1; j++) {
            if(!isFeatureInSet(currentFeatures, j)) {
                // std::cout << "\tConsidering adding the " << j << " feature\n";
                double accuracy = leaveOneOutCrossValidation(data, currentFeatures, j, add);
                std::cout << "\t\tUsing feature(s) {";
                for(size_t k = 0; k < currentFeatures.size()-1; k++) {
                    std::cout << currentFeatures.at(k) << ", ";
                }
                std::cout << currentFeatures.back() << "} accuracy is " << accuracy << "%\n";
                if(accuracy > bestSoFarAccuracy) {
                    bestSoFarAccuracy = accuracy;
                    featureToAddAtThisLevel = j;
                }
                currentFeatures.pop_back();
            }
        }
        currentFeatures.push_back(featureToAddAtThisLevel);
        // std::cout << "On level " << i << " I added feature " << featureToAddAtThisLevel << " to current set\n";
        std::cout << "\nFeature set {";
        for(size_t k = 0; k < currentFeatures.size()-1; k++) {
            std::cout << currentFeatures.at(k) << ", ";
        }
        std::cout << currentFeatures.back() << "} was best, accuracy is " << bestSoFarAccuracy << "%\n\n";
        bestFeatures.push_back(currentFeatures);
        bestFeaturesAccuracy.push_back(bestSoFarAccuracy);
        // if(bestSoFarAccuracy > bestAccuracy) {
        //     bestAccuracy = bestSoFarAccuracy;
        // }else {
        //     std::cout << "Warning: Accuracy has decreased! Continuing search in case of local maxima from " << bestAccuracy << "% to " << bestSoFarAccuracy << "%\n";
        //     currentFeatures.pop_back();
        // }
    }
    // std::cout << "Finished search!! The best feature subset is ";
    // for(size_t i = 0; i < currentFeatures.size(); i++) {
    //     std::cout << currentFeatures.at(i) << " ";
    // }
    // std::cout << "which has an accuracy of " << bestAccuracy << "%\n";
    bool decreased = false;
    bestAccuracy = 0;
    int bestFeatureSet = bestFeaturesAccuracy.front();
    for(size_t i = 0; i < bestFeaturesAccuracy.size()-1; i++) {
        if(bestFeaturesAccuracy.at(i+1) > bestAccuracy) {
            bestAccuracy = bestFeaturesAccuracy.at(i+1);
            bestFeatureSet = i+1;
        }
        if(bestFeaturesAccuracy.at(i+1) < bestFeaturesAccuracy.at(i)) {
            decreased = true;
        }
    }
    if(decreased) {
        std::cout << "{Warning: Accuracy has decreased!}";
    }
    std::cout << "\nFinished search!! The best feature subset is {";
    for(size_t i = 0; i < bestFeatures.at(bestFeatureSet).size()-1; i++) {
        std::cout << bestFeatures.at(bestFeatureSet).at(i) << ", ";
    }
    std::cout << bestFeatures.at(bestFeatureSet).back() << "} which has an accuracy of " << bestAccuracy << "%\n";
}

void randomEval(const std::vector<std::vector<double>> data){
    unsigned correct = 0;
    for(size_t i = 0; i < data.at(0).size(); i++){
        if(data.at(0).at(i) == rand() % 2 + 1){
            correct++;
        }
    }
    std::cout << "Using no features and \"random\" evaluation, I get an accuracy of " << static_cast<double>(correct)/data.at(0).size()*100 << "%\n\n";
}

void backwardSelection(std::vector<std::vector<double>> data, const int featureNum) {
    std::vector<int> currentFeatures;
    for(size_t i = 1; i < featureNum+1; i++) {
        currentFeatures.push_back(i);
    }
    std::string line = "";
    double bestAccuracy = 0;
    std::vector<std::vector<int>> bestFeatures;
    std::vector<int> bestFeaturesAccuracy;
    std::cout << "Beginning search.\n\n";
    size_t i = 0;
    for(size_t i = 1; i < featureNum+1; i++){
        // std::cout << "On the " << i << "th level of the search tree\n";
        size_t featureToRemoveAtThisLevel = 0;
        double bestSoFarAccuracy = 0;
        for(size_t j = 1; j < featureNum+1; j++) {
            if(isFeatureInSet(currentFeatures, j)) {
                // std::cout << "\tConsidering removing the " << j << " feature\n";
                //We're setting the feature to -1 because we're not adding any new
                //features. By setting featureToAdd to -1, we achieve that.
                double accuracy = leaveOneOutCrossValidation(data, currentFeatures, j, del);
                std::cout << "\t\tUsing feature(s) {";
                for(size_t k = 0; k < currentFeatures.size()-1; k++) {
                    std::cout << currentFeatures.at(k) << ", ";
                }
                std::cout << currentFeatures.back() << "} accuracy is " << accuracy << "%\n";
                if(accuracy > bestSoFarAccuracy) {
                    bestSoFarAccuracy = accuracy;
                    featureToRemoveAtThisLevel = j;
                }
                currentFeatures.push_back(j);
            }
        }
        auto it = std::find(currentFeatures.begin(), currentFeatures.end(), featureToRemoveAtThisLevel);
        if(it != currentFeatures.end()) {
            currentFeatures.erase(it);
        }else{
            throw std::runtime_error("backwardSelection: Feature not found in current set");
        }
        // std::cout << "On level " << i << " I removed feature " << featureToRemoveAtThisLevel << " from current set\n";
        std::cout << "\nFeature set {";
        for(size_t k = 0; k < currentFeatures.size()-1; k++) {
            std::cout << currentFeatures.at(k) << ", ";
        }
        std::cout << currentFeatures.back() << "} was best, accuracy is " << bestSoFarAccuracy << "%\n\n";
        bestFeatures.push_back(currentFeatures);
        bestFeaturesAccuracy.push_back(bestSoFarAccuracy);
        if(currentFeatures.size() == 1) {
            break;
        }
    }
    bool decreased = false;
    bestAccuracy = 0;
    int bestFeatureSet = bestFeaturesAccuracy.front();
    for(size_t i = 0; i < bestFeaturesAccuracy.size()-1; i++) {
        if(bestFeaturesAccuracy.at(i+1) > bestAccuracy) {
            bestAccuracy = bestFeaturesAccuracy.at(i+1);
            bestFeatureSet = i+1;
        }
        if(bestFeaturesAccuracy.at(i+1) < bestFeaturesAccuracy.at(i)) {
            decreased = true;
        }
    }
    if(decreased) {
        std::cout << "{Warning: Accuracy has decreased!}";
    }
    std::cout << "\nFinished search!! The best feature subset is {";
    for(size_t i = 0; i < bestFeatures.at(bestFeatureSet).size()-1; i++) {
        std::cout << bestFeatures.at(bestFeatureSet).at(i) << ", ";
    }
    std::cout << bestFeatures.at(bestFeatureSet).back() << "} which has an accuracy of " << bestAccuracy << "%\n";
}