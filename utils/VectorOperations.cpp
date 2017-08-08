//
// Created by Timi on 6. 07. 2017.
//

#include <vector>
#include "../utils/VectorOperations.h"

float *addTwoVectors(float *vector_a, float *vector_b, int d) {
    float *temp = new float[d];
    for (int i = 0; i < d; i++) {
        temp[i] = vector_a[i] + vector_b[i];
    }
    return temp;
}

float *subtractTwoVectors(float *vector_a, float *vector_b, int d) {
    float *temp = new float[d];
    for (int i = 0; i < d; i++) {
        temp[i] = vector_a[i] - vector_b[i];
    }
    return temp;
}

float *multiplyVectorByScalar(float scalar, float *vector, int d) {
    float *temp = new float[d];
    for (int i = 0; i < d; i++) {
        temp[i] *= vector[i] * scalar;
    }
    return temp;
}

std::vector<float> mutationVectorOperation(std::vector<float> a1, std::vector<float> a2, std::vector<float> a3, float F, int dimen) {
    std::vector<float> temp;
    for (int i = 0; i < dimen; i++) {
        temp.push_back(a1[i] + (F * (a2[i] - a3[i])));
    }
    return temp;
}