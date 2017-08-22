//
// Created by timi on 30.7.2017.
//

#include <chrono>
#include <iostream>
#include <utility>
#include <iomanip>
#include "../entities/Robot.h"

#include "DifferentialEvolutionAlgorithm.h"
#include "../utils/VectorOperations.h"
#include "../utils/TestingMatricesUtils.h"


// CONSTRUCTOR
DifferentialEvolutionAlgorithm::DifferentialEvolutionAlgorithm(const Robot &ROBOT, const std::vector<float> &endpointB,
                                                               const int Np, const float Cr, const float F,
                                                               const int Gmax, const int cntProbeLmt,
                                                               const float desiredError, const unsigned long seed)
        : ROBOT(ROBOT),
          D(ROBOT.getDimension()),
          endpointB(endpointB),
          Np(Np),
          Cr(Cr),
          F(F),
          Gmax(Gmax),
          cntProbeLmt(cntProbeLmt),
          desiredError(desiredError),
          seed(seed) {

    for (auto &arm: this->ROBOT.getArms()) {
        min_bounds.push_back(arm.getMinAngle());
        min_bounds.push_back(arm.getMinLength());
        max_bounds.push_back(arm.getMaxAngle());
        max_bounds.push_back(arm.getMaxLength());
    }

    RANDOM_0_1.param(std::uniform_real_distribution<>::param_type{0.0, 1.0});
    RANDOM_0_NP.param(std::uniform_int_distribution<>::param_type{0, Np - 1});
}

// METHODS
void DifferentialEvolutionAlgorithm::initialize(const int runNumber) {
    random_engine.seed(seed + runNumber);
    population.clear();

    std::vector<float> temp;
    for (int i = 0; i < Np; i++) {
        temp.clear();
        for (int j = 0; j < D; j++) {
            temp.push_back(setInitialIndividualValue(j));
        }
        population.push_back(temp);
    }
}

float DifferentialEvolutionAlgorithm::setInitialIndividualValue(const int index) {
    return min_bounds[index] +
           (float) RANDOM_0_1(random_engine) *
           (max_bounds[index] - min_bounds[index]);
}

Result DifferentialEvolutionAlgorithm::begin(const int runNumber) {
    initialize(runNumber);

    int generation = 0;
    int cntProbe = 0;
    float error = std::numeric_limits<float>::max();

    std::vector<float> parentVector;
    std::vector<float> mutationVector;
    std::vector<float> trialVector;

    clock_t timeBegin = clock();

    do {
        for (int i = 0; i < Np; i++) {
            parentVector = population[i];

            // mutation
            if (mutationVector.size() > 0) mutationVector.clear();

            int r1, r2, r3;
            do r1 = RANDOM_0_NP(random_engine); while (r1 == i);
            do r2 = RANDOM_0_NP(random_engine); while (r2 == i || r2 == r1);
            do r3 = RANDOM_0_NP(random_engine); while (r3 == i || r3 == r2 || r3 == r1);

            mutationVector = mutationVectorOperation(population[r1],
                                                     population[r2],
                                                     population[r3],
                                                     F,
                                                     D);


            // crossover
            if (trialVector.size() > 0) trialVector.clear();

            for (int j = 0; j < D; j++) {
                if ((float) RANDOM_0_1(random_engine) <= Cr) {
                    if (mutationVector[j] < min_bounds[j]) {
                        float temp = fabs(min_bounds[j] - mutationVector[j]);
                        temp = temp - (floorf(temp / fabs(max_bounds[j] - min_bounds[j])) * fabs(max_bounds[j] - min_bounds[j]));
                        RANDOM_0_N.param(std::uniform_real_distribution<>::param_type{0.0, temp});
                        trialVector.push_back(min_bounds[j] + (float) RANDOM_0_N(random_engine));
                    } else if (mutationVector[j] > max_bounds[j]) {
                        float temp = fabs(min_bounds[j] - mutationVector[j]);
                        temp = temp - (floorf(temp / fabs(max_bounds[j] - min_bounds[j])) * fabs(max_bounds[j] - min_bounds[j]));
                        RANDOM_0_N.param(std::uniform_real_distribution<>::param_type{0.0, temp});
                        trialVector.push_back(max_bounds[j] - (float) RANDOM_0_N(random_engine));
                    } else {
                        trialVector.push_back(mutationVector[j]);
                    }
                } else {
                    trialVector.push_back(parentVector[j]);
                }
            }

            //selection
            float trialError = fitnessFunction(trialVector, &cntProbe);
            float parentError = fitnessFunction(parentVector, &cntProbe);

            if (trialError <= parentError) {
                population[i] = trialVector;
                if (trialError < error) {
                    error = trialError;
                    bestIndividual = trialVector;
                }
            }
        }
    } while (shouldContinue(++generation, cntProbe, error));
    clock_t timeEnd = clock();

    return Result(double(timeEnd - timeBegin) / CLOCKS_PER_SEC,
                  error,
                  cntProbe,
                  bestIndividual);
}

float DifferentialEvolutionAlgorithm::fitnessFunction(std::vector<float> vector, int *cntProbe) {

    std::vector<std::vector<float>> endpointA = MatricesUtils::getIdentityMatrix();
    for (int i = 0; i < vector.size(); i += 2) {
        endpointA = MatricesUtils::multiply(endpointA,
                                         ROBOT.getArms()[i / 2].getTransformationMatrix(vector[i], vector[i + 1]));
    }
    endpointA = MatricesUtils::multiply(endpointA, MatricesUtils::getOrigin());

    float sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += pow(endpointA[i][0] - endpointB[i], 2);
    }

    if (cntProbe != nullptr) {
        *cntProbe = *cntProbe + 1;
    }

    return std::sqrt(sum);
}

bool DifferentialEvolutionAlgorithm::shouldContinue(int generation, int cntProbe, float error) {
    return generation <= Gmax && cntProbe < cntProbeLmt && error > desiredError;
}