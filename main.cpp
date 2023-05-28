#include <iostream>
#include <cstdlib>
#include <ctime>

int leaveOneOutCrossValidation(int data, int features, int algorithm) {
    // TODO: Implement leave one out cross validation
    return rand() % 101; // Generate a random accuracy between 0 and 100
}

int main() {
    std::cout << "Welcome to Raymond Yuan, Tangyuan Liang, and Miguelangel Tinoco's Feature Selection Algorithm.\n";

    int features;
    std::cout << "Please enter the total number of features: ";
    std::cin >> features;

    int algorithm;
    std::cout << "Type the number of the algorithm you want to run:\n"
              << "\t1) Forward Selection\n"
              << "\t2) Backward Elimination\n"
              << "\t3) Special Algorithm\n";
    std::cin >> algorithm;

    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    int accuracy = 0;
    accuracy = leaveOneOutCrossValidation(0, features, algorithm);
    std::cout << "Using no features and 'random' evaluation, I get an accuracy of " << accuracy << "%\n";

    return 0;
}