//
// Created by timi on 30.7.2017.
//

#include <chrono>
#include <iostream>
#include "../entities/Robot.h"

#include "OptimizedDE.h"
#include "../utils/VectorOperations.h"


// CONSTRUCTOR
OptimizedDE::OptimizedDE(Robot &robot, const int POPULATION_SIZE, const int MAX_GENERATION,
                         const float CROSSOVER_RATE, const float MUTATION_FACTOR) :
        ROBOT(robot),
        POPULATION_SIZE(POPULATION_SIZE),
        MAX_GENERATION(MAX_GENERATION),
        DIMENSION(ROBOT.getDimension()),
        CROSSOVER_RATE(CROSSOVER_RATE),
        MUTATION_FACTOR(MUTATION_FACTOR) {

    for (auto &arm: ROBOT.getArms()) {
        min_bounds.push_back(arm.getMinAngle());
        min_bounds.push_back(arm.getMinLength());
        max_bounds.push_back(arm.getMaxAngle());
        max_bounds.push_back(arm.getMaxLength());
    }

    random_engine.seed((unsigned long) std::chrono::system_clock::now().time_since_epoch().count());
    RANDOM_0_1.param(std::uniform_real_distribution<>::param_type{0.0, 1.0});
    RANDOM_0_NP.param(std::uniform_int_distribution<>::param_type{0, POPULATION_SIZE - 1});
}

// METHODS
void OptimizedDE::initialize() {
    population.clear();

    std::vector<float> temp;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        temp.clear();
        for (int j = 0; j < DIMENSION; j++) {
            temp.push_back(setInitialIndividualValue(j));
        }
        population.push_back(temp);
    }

    bestIndividualFitness = std::numeric_limits<float>::max();
}

void OptimizedDE::begin(std::vector<float> wantedEndpoint) {
    this->wantedEndpoint = wantedEndpoint;
    initialize();

    for (int generation = 0; generation < MAX_GENERATION; generation++) {
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
            float trial = fitnessFunction(trial_vectors[i]);
            float parent = fitnessFunction(population[i]);
            if (trial <= parent) {
                std::swap(population[i], trial_vectors[i]);
                if (trial < bestIndividualFitness) {
                    bestIndividualFitness = trial;
                    bestIndividual = population[i];
                }
            }
        }
        printf("\t\t> CURRENT (gen %d): %.2f mm\n", generation, bestIndividualFitness);
    }
    printf("\t> BEST: %.2f mm\n", bestIndividualFitness);

    for (int i = 0; i < ROBOT.getDimension(); i += 2) {
        ROBOT.getArms()[i/2].setAngle(bestIndividual[i]);
        ROBOT.getArms()[i/2].setLength(bestIndividual[i+1]);
    }
}

float OptimizedDE::setInitialIndividualValue(const int index) {
    return min_bounds[index] +
           (float) RANDOM_0_1(random_engine) *
           (max_bounds[index] - min_bounds[index]);
}

float OptimizedDE::fitnessFunction(std::vector<float> vector) {

    std::vector<std::vector<float>> result = MatricesUtils::getIdentityMatrix();
    for (int i = 0; i < vector.size(); i += 2) {
        result = MatricesUtils::multiply(result, ROBOT.getArms()[i / 2].getTransformationMatrix(vector[i], vector[i + 1]));
    }
    result = MatricesUtils::multiply(result, MatricesUtils::getOrigin());

    float sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += pow(result[i][0] - wantedEndpoint[i], 2);
    }
    return std::sqrt(sum);
}