# CS170_Project
# AI Feature Selection Algorithms
This repository contains the implementation of feature selection algorithms for data analysis in the field of artificial intelligence. The project focuses on forward selection and backward elimination algorithms to identify the optimal feature combinations that yield high accuracy results.

# Project Overview
The main objective of this project is to develop efficient algorithms for feature selection, which play a crucial role in improving the performance and interpretability of machine learning models. By utilizing forward selection and backward elimination techniques, we aim to identify the most relevant features from the input dataset.

# Key Features
Forward Selection: The forwardSelection() function starts with an empty feature set and iteratively adds features that result in the highest accuracy. It ensures that no duplicate features are added and maintains a record of the best feature set.

Backward Elimination: The backwardSelection() function begins with all available features and removes one feature at a time based on the impact on accuracy. Similar to forward selection, it tracks the best feature set throughout the elimination process.

Data Parsing: The parseData() function efficiently reads and organizes input data into a 2D vector called data. This ensures proper handling and compatibility with the feature selection algorithms.

Accuracy Evaluation: The leaveOneOutCrossValidation() function measures the accuracy of feature addition and removal. It modifies the feature set accordingly and returns the accuracy of the results.

# Contributions

Tangyuan Liang: Helped with the report. Implemented data parsing, backward selection, forward
selection, normalization, cross-validation, and random evaluation.  

Raymond Yuan: Report, dataset testing, debugging, performance optimization
  
Miguelangel Tinoco: Data Parsing, dataset testing, helped in dataset plotting, algorithm optimizations.
