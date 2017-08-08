//
// Created by timi on 7.8.2017.
//

#include "MatricesUtils.h"
#include "Trigonometry.h"


std::vector<std::vector<float>> MatricesUtils::getRotationAboutX(float angle) {
    return std::vector<std::vector<float>> {{1,     0,             0,              0},
                                            {0,     cos(angle),    -sin(angle),    0},
                                            {0,     sin(angle),    cos(angle),     0},
                                            {0,     0,             0,              1}};
}

std::vector<std::vector<float>> MatricesUtils::getRotationAboutY(float angle) {
    return std::vector<std::vector<float>> {{cos(angle),    0,      sin(angle),     0},
                                            {0,             1,      0,              0},
                                            {-sin(angle),   0,      cos(angle),     0},
                                            {0,             0,      0,              1}};
}

std::vector<std::vector<float>> MatricesUtils::getRotationAboutZ(float angle) {
    return std::vector<std::vector<float>> {{cos(angle),    -sin(angle),    0,      0},
                                            {sin(angle),    cos(angle),     0,      0},
                                            {0,             0,              1,      0},
                                            {0,             0,              0,      1}};
}

std::vector<std::vector<float>> MatricesUtils::getTranslationY(float length) {
    return std::vector<std::vector<float>> {{1,     0,      0,      0},
                                            {0,     1,      0,      length},
                                            {0,     0,      1,      0},
                                            {0,     0,      0,      1}};
}

std::vector<std::vector<float>>
MatricesUtils::multiply(std::vector<std::vector<float>> A, std::vector<std::vector<float>> B) {
    std::vector<std::vector<float>> product;
    std::vector<float> tempRow;

    for (int row = 0; row < A.size(); row++) {
        tempRow.clear();
        for (int col = 0; col < B[0].size(); col++) {
            tempRow.push_back(0);
            for (int inner = 0; inner < B.size(); inner++) {
                tempRow[col] += A[row][inner] * B[inner][col];
            }
        }
        product.push_back(tempRow);
    }
    return product;
}

std::vector<std::vector<float>> MatricesUtils::getIdentityMatrix() {
    return std::vector<std::vector<float>> {{1,     0,      0,      0},
                                            {0,     1,      0,      0},
                                            {0,     0,      1,      0},
                                            {0,     0,      0,      1}};
}

std::vector<std::vector<float>> MatricesUtils::getOrigin() {
    return std::vector<std::vector<float>> {{0},
                                            {0},
                                            {0},
                                            {1}};
}


