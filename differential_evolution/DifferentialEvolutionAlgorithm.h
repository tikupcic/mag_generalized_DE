//
// Created by timi on 30.7.2017.
//

#ifndef MAG_DE_OPTIMIZEDDE_H
#define MAG_DE_OPTIMIZEDDE_H

#include <random>
#include "../entities/Robot.h"
#include "../entities/Result.h"

class DifferentialEvolutionAlgorithm {

private:
    // problem
    Robot ROBOT;
    std::vector<float> endpointB;

    // krmilni parametri
    const int Np;
    const float Cr;
    const float F;

    //termination variables
    const int Gmax;
    const int cntProbeLmt;
    const float desiredError;

    //misc
    const unsigned long seed;
    const int D;

    std::vector<float> min_bounds;
    std::vector<float> max_bounds;

    std::default_random_engine random_engine;
    std::uniform_real_distribution<double> RANDOM_0_1;
    std::uniform_int_distribution<int> RANDOM_0_NP;
    std::uniform_real_distribution<double> RANDOM_0_N;

    std::vector<std::vector<float>> population;

    std::vector<float> bestIndividual;

public:
    DifferentialEvolutionAlgorithm(const Robot &ROBOT, const std::vector<float> &endpointB, const int Np,
                                   const float Cr, const float F, const int Gmax, const int cntProbeLmt,
                                   const float desiredError, const unsigned long seed);

    void initialize(const int runNumber);

    float setInitialIndividualValue(const int index);

    Result begin(const int runNumber);

    float fitnessFunction(std::vector<float> vector, int *cntProbe);

    bool shouldContinue(int generation, int cntProbe, float error);


};

#endif //MAG_DE_OPTIMIZEDDE_H