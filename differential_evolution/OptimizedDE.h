//
// Created by timi on 30.7.2017.
//

#ifndef MAG_DE_OPTIMIZEDDE_H
#define MAG_DE_OPTIMIZEDDE_H

#include <random>
#include "../entities/Robot.h"

class OptimizedDE {

private:
    Robot &ROBOT;

    const int POPULATION_SIZE;        // NP
    const int MAX_GENERATION;         // Gmax
    const int DIMENSION;              // D
    const float CROSSOVER_RATE;       // C
    const float MUTATION_FACTOR;      // F

    std::vector<float> min_bounds;
    std::vector<float> max_bounds;

    std::default_random_engine random_engine;
    std::uniform_real_distribution<double> RANDOM_0_1;
    std::uniform_int_distribution<int> RANDOM_0_NP;

    std::vector<std::vector<float>> population;
    std::vector<std::vector<float>> donor_vectors;
    std::vector<std::vector<float>> trial_vectors;
    std::vector<float> wantedEndpoint;

    float bestIndividualFitness;
    std::vector<float> bestIndividual;

public:
    OptimizedDE(Robot &robot, const int POPULATION_SIZE, const int MAX_GENERATION, const float CROSSOVER_RATE,
                const float MUTATION_FACTOR);

    float setInitialIndividualValue(const int index);

    float fitnessFunction(std::vector<float> vector);

    void initialize();

    void begin(std::vector<float> wantedEndpoint);
};

#endif //MAG_DE_OPTIMIZEDDE_H
