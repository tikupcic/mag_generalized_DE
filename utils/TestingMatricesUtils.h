//
// Created by Timi on 9. 08. 2017.
//

#ifndef GENERALIZED_ROBOT_ALLPAIRSTESTINGMATRICES_H
#define GENERALIZED_ROBOT_ALLPAIRSTESTINGMATRICES_H

#include <vector>

// Class containing All-Pairs testing matrices for number of various number of parameters
class TestingMatricesUtils {

public:
    static std::vector<std::vector<float>> getAllPairsMatrix(const int numberOfParameters);

    static std::vector<std::vector<float>> getP2Matrix();
    static std::vector<std::vector<float>> getP3Matrix();
    static std::vector<std::vector<float>> getP4Matrix();
    static std::vector<std::vector<float>> getP5Matrix();
    static std::vector<std::vector<float>> getP6Matrix();
    static std::vector<std::vector<float>> getP7Matrix();
    static std::vector<std::vector<float>> getP8Matrix();
    static std::vector<std::vector<float>> getP9Matrix();
    static std::vector<std::vector<float>> getP10Matrix();
    static std::vector<std::vector<float>> getP11Matrix();
    static std::vector<std::vector<float>> getP12Matrix();
};

#endif //GENERALIZED_ROBOT_ALLPAIRSTESTINGMATRICES_H