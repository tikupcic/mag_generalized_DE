//
// Created by Timi on 22. 08. 2017.
//

#include <iostream>
#include <iomanip>
#include "Result.h"

Result::Result(double elapsedTime, float error, int probeLmt, const std::vector<float> &bestIndividual) :
        elapsedTime(elapsedTime),
        error(error),
        probeLmt(probeLmt),
        bestIndividual(bestIndividual) {}

Result Result::operator+(const Result &r) {
    return Result(this->elapsedTime + r.elapsedTime,
                  this->error + r.error,
                  this->probeLmt + r.probeLmt,
                  std::vector<float>());
}

void Result::getAverage(const int runNum) {
    elapsedTime /= runNum;
    error /= runNum;
    probeLmt /= runNum;
    print();
}

void Result::print() {
    std::cout << "> Elapsed time: " << elapsedTime << "s" << std::endl;
    std::cout << "> Error: " << error << "mm" << std::endl;
    std::cout << "> ProbeLmt: " << probeLmt << std::endl;
    if (bestIndividual.size() > 0) {
        std::cout << "> Best settings: ";
        for (float val : bestIndividual) {
            std::cout << std::fixed << std::setprecision(3) << val << ",";
        }
        std::cout << std::endl;
    }
}
