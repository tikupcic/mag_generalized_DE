//
// Created by timi on 30.7.2017.
//

#include <chrono>
#include <iostream>
#include <utility>
#include "../entities/Robot.h"

#include "DifferentialEvolutionAlgorithm.h"
#include "../utils/VectorOperations.h"
#include "../utils/TestingMatricesUtils.h"


// CONSTRUCTOR
DifferentialEvolutionAlgorithm::DifferentialEvolutionAlgorithm(const Robot &ROBOT, const int POPULATION_SIZE,
                                                               const int MAX_GENERATION,const float CROSSOVER_RATE,
                                                               const float MUTATION_FACTOR, const bool considerErrors,
                                                               const unsigned long seed, const float term_epsilonLimit,
                                                               const int term_cntProbeLimit, const double term_runtimeLimit,
                                                               const bool debugMode) :
        ROBOT(ROBOT),
        POPULATION_SIZE(POPULATION_SIZE),
        MAX_GENERATION(MAX_GENERATION),
        DIMENSION(ROBOT.getDimension()),
        CROSSOVER_RATE(CROSSOVER_RATE),
        MUTATION_FACTOR(MUTATION_FACTOR),
        considerErrors(considerErrors),
        seed(seed),
        term_epsilonLimit(term_epsilonLimit),
        term_cntProbeLimit(term_cntProbeLimit),
        term_runtimeLimit(term_runtimeLimit),
        debugMode(debugMode){

    for (auto &arm: this->ROBOT.getArms()) {
        min_bounds.push_back(arm.getMinAngle());
        min_bounds.push_back(arm.getMinLength());
        max_bounds.push_back(arm.getMaxAngle());
        max_bounds.push_back(arm.getMaxLength());
    }

    RANDOM_0_1.param(std::uniform_real_distribution<>::param_type{0.0, 1.0});
    RANDOM_0_NP.param(std::uniform_int_distribution<>::param_type{0, POPULATION_SIZE - 1});
}

// METHODS
void DifferentialEvolutionAlgorithm::initialize(const int runNumber) {
    random_engine.seed(seed + runNumber);
    population.clear();

    std::vector<float> temp;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        temp.clear();
        for (int j = 0; j < DIMENSION; j++) {
            temp.push_back(setInitialIndividualValue(j));
        }
        population.push_back(temp);
    }
}

std::vector<float> DifferentialEvolutionAlgorithm::begin(std::vector<float> wantedEndpoint, const int runNumber) {
    this->wantedEndpoint = std::move(wantedEndpoint);
    initialize(runNumber);

    float epsilon = std::numeric_limits<float>::max();
    clock_t runtime = 0;
    int cntProbe = 0;

    for (int generation = 0;; generation++) {
        donor_vectors.clear();
        trial_vectors.clear();

        // mutation
        int r1, r2, r3;
        for (int i = 0; i < POPULATION_SIZE; i++) {
            do r1 = RANDOM_0_NP(random_engine); while (r1 == i);
            do r2 = RANDOM_0_NP(random_engine); while (r2 == i || r2 == r1);
            do r3 = RANDOM_0_NP(random_engine); while (r3 == i || r3 == r2 || r3 == r1);

            donor_vectors.push_back(
                    mutationVectorOperation(population[r1],
                                            population[r2],
                                            population[r3],
                                            MUTATION_FACTOR,
                                            DIMENSION));
        }

        // crossover
        for (int i = 0; i < POPULATION_SIZE; i++) {
            std::vector<float> temp;
            for (int j = 0; j < DIMENSION; j++) {
                if ((float) RANDOM_0_1(random_engine) <= CROSSOVER_RATE) {
                    if (donor_vectors[i][j] < min_bounds[j]) {
                        temp.push_back(min_bounds[j]);
                    } else if (donor_vectors[i][j] > max_bounds[j]) {
                        temp.push_back(max_bounds[j]);
                    } else {
                        temp.push_back(donor_vectors[i][j]);
                    }
                } else {
                    temp.push_back(population[i][j]);
                }
            }
            trial_vectors.push_back(temp);
        }

        //selection
        for (int i = 0; i < POPULATION_SIZE; i++) {
            float trial = considerErrors ? pairwiseFitnessFunction(trial_vectors[i], &cntProbe) : fitnessFunction(
                    trial_vectors[i], &cntProbe);
            float parent = considerErrors ? pairwiseFitnessFunction(population[i], &cntProbe) : fitnessFunction(population[i], &cntProbe);
            if (trial <= parent) {
                std::swap(population[i], trial_vectors[i]);
                if (trial < epsilon) {
                    epsilon = trial;
                    bestIndividual = population[i];
                }
            }
        }

        if(debugMode)
            printf("\t> Current best: %.2f mm {gen: %d, time: %.2f, cntProbe: %d}\n", epsilon, generation, double(clock() - runtime) / CLOCKS_PER_SEC, cntProbe);

        //termination
        if(shouldTerminate(generation, epsilon, runtime, cntProbe)) {
            break;
        }
    }

    return bestIndividual;
}

float DifferentialEvolutionAlgorithm::setInitialIndividualValue(const int index) {
    return min_bounds[index] +
           (float) RANDOM_0_1(random_engine) *
           (max_bounds[index] - min_bounds[index]);
}

float DifferentialEvolutionAlgorithm::fitnessFunction(std::vector<float> vector, int *cntProbe) {

    std::vector<std::vector<float>> result = MatricesUtils::getIdentityMatrix();
    for (int i = 0; i < vector.size(); i += 2) {
        result = MatricesUtils::multiply(result,
                                         ROBOT.getArms()[i / 2].getTransformationMatrix(vector[i], vector[i + 1]));
    }
    result = MatricesUtils::multiply(result, MatricesUtils::getOrigin());

    float sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += pow(result[i][0] - wantedEndpoint[i], 2);
    }

    if(cntProbe != nullptr) {
        *cntProbe = *cntProbe + 1;
    }

    return std::sqrt(sum);
}

float DifferentialEvolutionAlgorithm::pairwiseFitnessFunction(std::vector<float> vector, int *cntProbe) {
    std::vector<std::vector<float>> pairwiseMatrix = TestingMatricesUtils::getAllPairsMatrix(ROBOT.getDimension() / 2);
    std::vector<float> temp;
    float errorSum = 0;
    for (int i = 0; i < pairwiseMatrix.size(); i++) {
        temp.clear();
        for (int j = 0; j < vector.size(); j++) {
            if (j % 2 == 0) {
                temp.push_back(vector[j] + (sqrt(pow(ROBOT.getArms()[j / 2].getMaxAngle(), 2) +
                                                 pow(ROBOT.getArms()[j / 2].getMinAngle(), 2)) *
                                            ROBOT.getArms()[j / 2].getError() * pairwiseMatrix[i][j / 2]));
            } else {
                temp.push_back(vector[j]);
            }
        }
        errorSum += fitnessFunction(temp, cntProbe);
    }
    return errorSum / pairwiseMatrix.size();
}

bool DifferentialEvolutionAlgorithm::shouldTerminate(int generation, float epsilon, clock_t runtime, int cntProbe) {
    bool terminate = false;

    terminate |= generation >= MAX_GENERATION;
    terminate |= epsilon <= term_epsilonLimit;
    terminate |= double(clock() - runtime) / CLOCKS_PER_SEC >= term_runtimeLimit;
    terminate |= cntProbe >= term_cntProbeLimit;

    return terminate;
}
