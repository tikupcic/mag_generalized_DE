//
// Created by Timi on 9. 08. 2017.
//

#include "TestingMatricesUtils.h"

std::vector<std::vector<float>> TestingMatricesUtils::getAllPairsMatrix(const int numberOfParameters) {
    switch (numberOfParameters) {
        case 2:
            return getP2Matrix();
        case 3:
            return getP3Matrix();
        case 4:
            return getP4Matrix();
        case 5:
            return getP5Matrix();
        case 6:
            return getP6Matrix();
        case 7:
            return getP7Matrix();
        case 8:
            return getP8Matrix();
        case 9:
            return getP9Matrix();
        case 10:
            return getP10Matrix();
        case 11:
            return getP11Matrix();
        case 12:
            return getP12Matrix();
    }
}

std::vector<std::vector<float>> TestingMatricesUtils::getP2Matrix() {
    return std::vector<std::vector<float>> {{1,  -1},
                                            {1,  1},
                                            {-1, 1},
                                            {-1, -1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP3Matrix() {
    return std::vector<std::vector<float>> {{1,  -1, 1},
                                            {1,  1,  -1},
                                            {-1, -1, -1},
                                            {-1, 1,  1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP4Matrix() {
    return std::vector<std::vector<float>> {{-1, 1,  1,  1},
                                            {-1, -1, -1, -1},
                                            {1,  -1, 1,  -1},
                                            {1,  1,  -1, -1},
                                            {1,  -1, -1, 1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP5Matrix() {
    return std::vector<std::vector<float>> {{1,  1,  -1, -1, 1},
                                            {-1, -1, 1,  1,  -1},
                                            {1,  -1, 1,  1,  1},
                                            {-1, 1,  -1, 1,  -1},
                                            {1,  -1, 1,  -1, -1},
                                            {1,  -1, 1,  -1, -1},
                                            {-1, -1, -1, -1, -1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP6Matrix() {
    return std::vector<std::vector<float>> {{1,  1,  -1, 1,  1,  1},
                                            {-1, -1, 1,  -1, -1, -1},
                                            {1,  -1, 1,  -1, 1,  1},
                                            {1,  1,  1,  1,  -1, -1},
                                            {-1, -1, -1, 1,  1,  -1},
                                            {-1, 1,  -1, -1, -1, 1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP7Matrix() {
    return std::vector<std::vector<float>> {{1,  1,  1,  -1, 1,  1,  1},
                                            {-1, -1, -1, -1, -1, -1, -1},
                                            {1,  1,  -1, 1,  1,  -1, -1},
                                            {-1, -1, 1,  1,  -1, -1, 1},
                                            {-1, -1, 1,  1,  1,  1,  -1},
                                            {1,  -1, -1, 1,  -1, 1,  1},
                                            {-1, 1,  1,  -1, -1, -1, 1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP8Matrix() {
    return std::vector<std::vector<float>> {{1,  1,  1,  -1, -1, -1, -1, 1},
                                            {-1, -1, -1, 1,  1,  1,  1,  1},
                                            {-1, 1,  -1, -1, 1,  -1, 1,  -1},
                                            {-1, -1, 1,  1,  -1, 1,  -1, -1},
                                            {1,  -1, -1, 1,  -1, -1, 1,  -1},
                                            {1,  -1, 1,  -1, 1,  1,  1,  -1},
                                            {1,  1,  -1, 1,  1,  1,  -1, -1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP9Matrix() {
    return std::vector<std::vector<float>> {{1,  -1, 1,  -1, 1,  1,  -1, 1,  1},
                                            {-1, 1,  -1, 1,  -1, -1, 1,  -1, -1},
                                            {-1, -1, -1, -1, -1, -1, -1, 1,  -1},
                                            {1,  1,  1,  1,  1,  -1, 1,  -1, 1},
                                            {-1, -1, 1,  1,  1,  1,  -1, -1, -1},
                                            {1,  1,  -1, -1, -1, 1,  1,  1,  -1},
                                            {-1, -1, -1, 1,  -1, -1, 1,  1,  1},
                                            {1,  1,  1,  -1, -1, 1,  -1, -1, -1},
                                            {1,  1,  -1, 1,  1,  1,  -1, 1,  1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP10Matrix() {
    return std::vector<std::vector<float>> {{1,  -1, 1,  -1, -1, -1, 1,  1,  -1, -1},
                                            {-1, 1,  -1, 1,  1,  1,  1,  -1, 1,  1},
                                            {-1, -1, -1, -1, 1,  1,  -1, -1, -1, -1},
                                            {1,  1,  1,  1,  1,  -1, -1, 1,  1,  1},
                                            {-1, -1, -1, -1, -1, -1, -1, -1, 1,  1},
                                            {-1, 1,  1,  1,  -1, 1,  -1, 1,  1,  -1},
                                            {1,  1,  1,  1,  1,  1,  -1, -1, -1, 1},
                                            {1,  1,  -1, -1, 1,  1,  1,  1,  -1, 1},
                                            {-1, -1, -1, 1,  1,  -1, -1, 1,  1,  -1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP11Matrix() {
    return std::vector<std::vector<float>> {{1,  -1, -1, -1, -1, -1, 1,  -1, 1,  1,  1},
                                            {-1, 1,  1,  1,  1,  1,  -1, -1, -1, -1, -1},
                                            {1,  1,  -1, 1,  -1, 1,  1,  1,  1,  -1, -1},
                                            {-1, -1, 1,  -1, 1,  -1, -1, 1,  -1, 1,  -1},
                                            {1,  -1, 1,  -1, 1,  1,  1,  1,  -1, -1, 1},
                                            {-1, 1,  1,  1,  -1, -1, -1, -1, 1,  -1, 1},
                                            {-1, 1,  1,  1,  -1, -1, -1, -1, 1,  -1, 1},
                                            {1,  -1, -1, 1,  1,  1,  -1, -1, 1,  1,  -1},
                                            {-1, -1, -1, 1,  -1, -1, 1,  1,  -1, 1,  1}};
}

std::vector<std::vector<float>> TestingMatricesUtils::getP12Matrix() {
    return std::vector<std::vector<float>> {{-1, -1, 1,  1,  1,  1,  1,  -1, -1, 1,  -1, 1},
                                            {1,  1,  -1, -1, -1, -1, -1, 1,  -1, -1, 1,  -1},
                                            {-1, 1,  -1, -1, 1,  1,  -1, 1,  1,  1,  1,  1},
                                            {1,  -1, 1,  1,  -1, -1, 1,  1,  1,  -1, -1, -1},
                                            {-1, -1, -1, -1, -1, 1,  1,  -1, 1,  -1, 1,  -1},
                                            {1,  1,  -1, 1,  -1, -1, -1, -1, -1, 1,  -1, 1},
                                            {1,  1,  -1, 1,  -1, -1, -1, -1, -1, 1,  -1, 1},
                                            {1,  1,  1,  1,  1,  1,  1,  1,  -1, -1, 1,  1},
                                            {-1, 1,  -1, 1,  -1, -1, -1, 1,  -1, -1, -1, 1}};
}

