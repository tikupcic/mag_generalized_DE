//
// Created by Timi on 22. 08. 2017.
//

#ifndef MAG_GENERALIZED_DE_RESULT_H
#define MAG_GENERALIZED_DE_RESULT_H


#include <vector>

class Result {

public:
    double elapsedTime;
    float error;
    int probeLmt;
    std::vector<float> bestIndividual;

    Result(double elapsedTime, float error, int probeLmt, const std::vector<float> &bestIndividual);

    Result operator+(const Result& r);

    void getAverage(int runNum);

    void print();
};


#endif //MAG_GENERALIZED_DE_RESULT_H
