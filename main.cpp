
#include <iostream>
#include "entities/ArmSegment.h"
#include "entities/Robot.h"
#include "differential_evolution/DifferentialEvolutionAlgorithm.h"
#include "utils/arguments/argh.h"
#include "utils/arguments/Argument.h"
#include <typeinfo>
#include <fstream>

/******************** ORIENTATION ********************/
/*                                                   */
/*                     y  |         -z /             */
/*                        |          /               */
/*                        |        /                 */
/*                        |      /                   */
/*                        |    /                     */
/*                        |  /                       */
/*   _ _ _ _ _ _ _ _ _ _ _|/_ _ _ _._ _ _ _ _ _ _ x  */
/*   -x                  /|        A                 */
/*                     /  |                          */
/*                   /    |                          */
/*                 /      |                          */
/*               /        |               x  y  z    */
/*             /          |             A(5, 0, 0)   */
/*         z /            | -y                       */
/*                                      angles = CCW */
/*****************************************************/

// FLAGS
Argument<bool>            HELP("-h", "Shows help.");
Argument<bool>            DEBUG("-d", "Run as debug version (inner steps of algorithm are shown).");
Argument<bool>            CONSIDER_ERRORS("-e", "Run in error mode.");

// FILES
Argument<std::string>     ROBOT("--robot", "Load a file containing robot arm parameters (check example file for more info).");
Argument<std::string>     ENDPOINT("--endpoint", "Load a file with wanted endpoint (check example file for more info).");

// PARAMETERS
Argument<int>             NUMBER_OF_RUNS("--runs", "Number of individual runs for current robot. (integer)", 1);
Argument<int>             POPULATION_SIZE("--psize", "Set population size. (integer)", 15);
Argument<int>             MAX_GENERATION("--Gmax", "Set maximum number of generations.(integer)", 1000);
Argument<float>           MUTATION_FACTOR("--F", "Set mutation factor. (decimal)", .5f);
Argument<float>           CROSSOVER_RATE("--C", "Set crossover rate. (decimal)", .9f);
Argument<unsigned long>   SEED("--seed", "Set seed. (unsigned integer)", 35675);
Argument<float>           EPSILON_LIMIT("--elim", "Set eplison limit in millimeters (algorithm will terminate if reached). (decimal)", 3.5f);
Argument<int>             CNT_PROBE_LIMIT("--plim", "Set probe limit (algorithm will terminate if reached). (integer)", 10000);
Argument<double>          TIME_LIMIT("--tlim", "Set time limit in seconds (algorithm will terminate if reached). (decimal)", 10);

void showHelp();
void loadRobot(const std::string &fileName, std::vector<ArmSegment> &arms);
void loadEndpoint(const std::string &fileName, std::vector<float> &endpoint);
ArmSegment createSegment(std::vector<std::string> params);
ArmSegment::RotationOption parseRotation(const std::string param);
void printResults(const float &error, const float &pairwiseError, const double &elapsed);

int main(const int argc, const char* argv[]) {
    argh::parser argParser;
    argParser.parse(argc, argv, argh::parser::SINGLE_DASH_IS_MULTIFLAG);

    // check if should show help
    if (argc <= 3 || argParser[HELP.getArgument()]) {
        showHelp();
        exit(0);
    }

    // check for flags
    DEBUG.value = argParser["d"];
    CONSIDER_ERRORS.value = argParser["e"];

    // load robot from file
    std::vector<ArmSegment> arms;
    argParser(ROBOT.getArgument()) >> ROBOT.value;
    loadRobot(ROBOT.value, arms);
    Robot ROBOT(arms);

    // load endpoint from file
    std::vector<float> endPoint;
    argParser(ENDPOINT.getArgument()) >> ENDPOINT.value;
    loadEndpoint(ENDPOINT.value, endPoint);

    // check for values
    argParser(NUMBER_OF_RUNS.getArgument(), NUMBER_OF_RUNS.getDefValue()) >> NUMBER_OF_RUNS.value;
    argParser(POPULATION_SIZE.getArgument(), POPULATION_SIZE.getDefValue()) >> POPULATION_SIZE.value;
    argParser(MAX_GENERATION.getArgument(), MAX_GENERATION.getDefValue()) >> MAX_GENERATION.value;
    argParser(MUTATION_FACTOR.getArgument(), MUTATION_FACTOR.getDefValue()) >> MUTATION_FACTOR.value;
    argParser(CROSSOVER_RATE.getArgument(), CROSSOVER_RATE.getDefValue()) >> CROSSOVER_RATE.value;
    argParser(SEED.getArgument(), SEED.getDefValue()) >> SEED.value;
    argParser(EPSILON_LIMIT.getArgument(), EPSILON_LIMIT.getDefValue()) >> EPSILON_LIMIT.value;
    argParser(CNT_PROBE_LIMIT.getArgument(), CNT_PROBE_LIMIT.getDefValue()) >> CNT_PROBE_LIMIT.value;
    argParser(TIME_LIMIT.getArgument(), TIME_LIMIT.getDefValue()) >> TIME_LIMIT.value;

    DifferentialEvolutionAlgorithm algorithm(ROBOT,
                                             POPULATION_SIZE.value,
                                             MAX_GENERATION.value,
                                             CROSSOVER_RATE.value,
                                             MUTATION_FACTOR.value,
                                             CONSIDER_ERRORS.value,
                                             SEED.value,
                                             EPSILON_LIMIT.value,
                                             CNT_PROBE_LIMIT.value,
                                             TIME_LIMIT.value,
                                             DEBUG.value);

    // variables for overall average
    float sumElapsed = 0;
    float sumError = 0;
    float sumPairWiseError = 0;

    // variables for overall best individual of all runs
    std::vector<float> overallBestIndividual;
    float overallBestPairwiseError = std::numeric_limits<float>::max();
    float overallBestError = std::numeric_limits<float>::max();
    float overallBestElapsed;

    for (int n = 0; n < NUMBER_OF_RUNS.value; n++) {
        if (DEBUG.value)
            std::cout << "======================== Run: " << n + 1 << " begins ========================" << std::endl;

        clock_t begin = clock();
        std::vector<float> result = algorithm.begin(endPoint, n);
        clock_t end = clock();

        float pairwiseError = algorithm.pairwiseFitnessFunction(result, nullptr);
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        float error = algorithm.fitnessFunction(result, nullptr);

        sumPairWiseError += pairwiseError;
        sumElapsed += elapsed_secs;
        sumError += error;

        if (CONSIDER_ERRORS.value ? pairwiseError <= overallBestPairwiseError : error <= overallBestError) {
            overallBestIndividual = result;
            overallBestError = error;
            overallBestPairwiseError = pairwiseError;
            overallBestElapsed = elapsed_secs;
        }

        if (DEBUG.value) {
            std::cout << "======================== Run: " << n + 1 << " results ========================" << std::endl;
            printResults(error, pairwiseError, elapsed_secs);

            std::string raw = "";
            for (int i = 0; i < result.size(); i += 2) {
                printf("\n Segment %d : %.2f° / %.2f mm", i / 2 + 1, result[i], result[i + 1]);

                raw += std::to_string(result[i]) + "f," + std::to_string(result[i + 1]) + "f,";
            }
            std::cout << std::endl << std::endl << "raw data: " << raw << std::endl << std::endl;
        }
    }

    std::cout << "======================= OVERALL AVERAGE =======================" << std::endl;
    printResults(sumError / NUMBER_OF_RUNS.value, sumPairWiseError / NUMBER_OF_RUNS.value, sumElapsed / NUMBER_OF_RUNS.value);

    std::cout << std::endl <<  "=============== OVERALL BEST INDIVIDUAL RESULTS ===============" << std::endl;
    printResults(overallBestError, overallBestPairwiseError, overallBestElapsed);

    printf("> Run best w/o error: %.2f mm\n", overallBestError);
    printf("> Run best w/  error: %.2f mm\n", overallBestPairwiseError);
    printf("> Run elapsed time:   %.5f sec\n", overallBestElapsed);

    std::string raw = "";
    for (int i = 0; i < overallBestIndividual.size(); i += 2) {
        printf("\n Segment %d : %.2f° / %.2f mm", i / 2 + 1, overallBestIndividual[i], overallBestIndividual[i + 1]);

        raw += std::to_string(overallBestIndividual[i]) + "f," + std::to_string(overallBestIndividual[i + 1]) + "f,";
    }
    std::cout << std::endl << std::endl << "raw data: " << raw << std::endl << std::endl;

    return 0;
}

void showHelp() {

    std::cout << "============== <HELP> ==============" << std::endl;
    std::cout << "Defaults (if any) are listed in brackets []" << std::endl;

    std::cout << HELP.getArgument()             << " ~ " <<   HELP.getDescription() << std::endl;

    std::cout << "Flags:" << std::endl;
    std::cout << "\t" << DEBUG.getArgument()            << " ~ " <<   DEBUG.getDescription()  << std::endl;
    std::cout << "\t" << CONSIDER_ERRORS.getArgument()  << " ~ " <<   CONSIDER_ERRORS.getDescription()  << std::endl;

    std::cout << "Files:" << std::endl;
    std::cout << "\t" << ROBOT.getArgument()            << " ~ " <<   ROBOT.getDescription() << std::endl;
    std::cout << "\t" << ENDPOINT.getArgument()         << " ~ " <<   ENDPOINT.getDescription() << std::endl;

    std::cout << "Params:" << std::endl;
    std::cout << "\t" << NUMBER_OF_RUNS.getArgument()   << " ~ " <<   NUMBER_OF_RUNS.getDescription()   <<      NUMBER_OF_RUNS.defToString() << std::endl;
    std::cout << "\t" << POPULATION_SIZE.getArgument()  << " ~ " <<   POPULATION_SIZE.getDescription()  <<      POPULATION_SIZE.defToString() << std::endl;
    std::cout << "\t" << MAX_GENERATION.getArgument()   << " ~ " <<   MAX_GENERATION.getDescription()   <<      MAX_GENERATION.defToString() << std::endl;
    std::cout << "\t" << MUTATION_FACTOR.getArgument()  << " ~ " <<   MUTATION_FACTOR.getDescription()  <<      MUTATION_FACTOR.defToString() << std::endl;
    std::cout << "\t" << CROSSOVER_RATE.getArgument()   << " ~ " <<   CROSSOVER_RATE.getDescription()   <<      CROSSOVER_RATE.defToString() << std::endl;
    std::cout << "\t" << SEED.getArgument()             << " ~ " <<   SEED.getDescription()             <<      SEED.defToString() << std::endl;
    std::cout << "\t" << EPSILON_LIMIT.getArgument()    << " ~ " <<   EPSILON_LIMIT.getDescription()    <<      EPSILON_LIMIT.defToString() << std::endl;
    std::cout << "\t" << CNT_PROBE_LIMIT.getArgument()  << " ~ " <<   CNT_PROBE_LIMIT.getDescription()  <<      CNT_PROBE_LIMIT.defToString() << std::endl;
    std::cout << "\t" << TIME_LIMIT.getArgument()       << " ~ " <<   TIME_LIMIT.getDescription()       <<      TIME_LIMIT.defToString() << std::endl;

    std::cout << "============== </HELP> ==============" << std::endl;
}

void loadRobot(const std::string &fileName, std::vector<ArmSegment> &arms) {
    std::ifstream file(fileName);
    std::string line, param;
    std::vector<std::string> params;

    while(std::getline(file, line)) {
        std::istringstream s(line);
        params.clear();
        while (getline(s, param,',')) {
            params.push_back(param);
        }
        arms.push_back(createSegment(params));
    }
    file.close();
}

ArmSegment createSegment(std::vector<std::string> params) {
    if(params.size() == 4) {
        return ArmSegment(
                std::stof(params[0]),
                std::stof(params[1]),
                std::stof(params[2]),
                parseRotation(params[3]));
    } else if (params.size() == 6) {
        return ArmSegment(
                std::stof(params[0]),
                std::stof(params[1]),
                std::stof(params[2]),
                std::stof(params[3]),
                std::stof(params[4]),
                parseRotation(params[5]));
    }
}

ArmSegment::RotationOption parseRotation(const std::string param) {
    if(param.find("X_AXIS") != std::string::npos) {
        return ArmSegment::RotationOption::X_AXIS;
    } else if (param.find("Y_AXIS") != std::string::npos) {
        return ArmSegment::RotationOption::Y_AXIS;
    } else if (param.find("Z_AXIS")!= std::string::npos) {
        return ArmSegment::RotationOption::Z_AXIS;
    }
}

void loadEndpoint(const std::string &fileName, std::vector<float> &endpoint) {
    std::ifstream file(fileName);
    std::string line, param;

    while(std::getline(file, line)) {
        std::istringstream s(line);
        while (getline(s, param,',')) {
            endpoint.push_back(std::stof(param));
        }
    }
    file.close();
}

void printResults(const float &error, const float &pairwiseError, const double &elapsed) {
    printf("> Best w/o error: %.2f mm\n", error);
    printf("> Best w/  error: %.2f mm\n", pairwiseError);
    printf("> Elapsed time:   %.5f sec\n", elapsed);
}