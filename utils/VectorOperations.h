//
// Created by Timi on 6. 07. 2017.
//

#ifndef DE_IMPLEMENTATION_VECTOROPERATIONS_H
#define DE_IMPLEMENTATION_VECTOROPERATIONS_H

float *addTwoVectors(float *vector_a, float *vector_b, int d);

float *subtractTwoVectors(float *vector_a, float *vector_b, int d);

float *multiplyVectorByScalar(float scalar, float *vector, int d);

std::vector<float> mutationVectorOperation(std::vector<float> a1, std::vector<float> a2, std::vector<float> a3, float F, int dimen);

#endif //DE_IMPLEMENTATION_VECTOROPERATIONS_H
