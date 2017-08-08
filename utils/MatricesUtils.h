//
// Created by timi on 7.8.2017.
//

#ifndef GENERALIZED_ROBOT_MATRICESUTILS_H
#define GENERALIZED_ROBOT_MATRICESUTILS_H


#include <vector>

class MatricesUtils {

public:
    static std::vector<std::vector<float>> getRotationAboutX(float angle);
    static std::vector<std::vector<float>> getRotationAboutY(float angle);
    static std::vector<std::vector<float>> getRotationAboutZ(float angle);
    static std::vector<std::vector<float>> getTranslationY(float length);

    static std::vector<std::vector<float>> multiply(std::vector<std::vector<float>> A, std::vector<std::vector<float>> B);

    static std::vector<std::vector<float>> getIdentityMatrix();
    static std::vector<std::vector<float>> getOrigin();
};


#endif //GENERALIZED_ROBOT_MATRICESUTILS_H
