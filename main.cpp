
#include <iostream>
#include "entities/ArmSegment.h"
#include "entities/Robot.h"
#include "differential_evolution/DifferentialEvolutionAlgorithm.h"
#include "utils/arguments/argh.h"
#include "utils/arguments/Argument.h"
#include <typeinfo>
#include <fstream>
#include <iomanip>

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

// FILES
Argument<std::string>     ROBOT_CONF("--robot", "Load a file containing robot arm parameters (check example file for more info).");
Argument<std::string>     ENDPOINT_CONF("--endpoint", "Load a file with wanted endpoint (check example file for more info).");

// PARAMETERS
Argument<int>             NUMBER_OF_RUNS("--runs", "Number of individual runs for current robot. (integer)", 100);
Argument<int>             POPULATION_SIZE("--psize", "Set population size. (integer)", 100);
Argument<int>             MAX_GENERATION("--Gmax", "Set maximum number of generations.(integer)", 1000);
Argument<float>           MUTATION_FACTOR("--F", "Set mutation factor. (decimal)", .5f);
Argument<float>           CROSSOVER_RATE("--C", "Set crossover rate. (decimal)", .9f);
Argument<unsigned long>   SEED("--seed", "Set seed. (unsigned integer)", 1500);
Argument<float>           EPSILON_LIMIT("--elim", "Set eplison limit in millimeters (algorithm will terminate if reached). (decimal)", 1.0f);
Argument<int>             CNT_PROBE_LIMIT("--plim", "Set probe limit (algorithm will terminate if reached). (integer)", 100000);
Argument<double>          TIME_LIMIT("--tlim", "Set time limit in seconds (algorithm will terminate if reached). (decimal)", 0);

void showHelp();
void loadRobot(const std::string &fileName, std::vector<ArmSegment> &arms);
void loadEndpoint(const std::string &fileName, std::vector<float> &endpoint);
ArmSegment createSegment(std::vector<std::string> params);
ArmSegment::RotationOption parseRotation(const std::string param);
void startExperiment(DifferentialEvolutionAlgorithm algorithm);
void printResults(const float &error, const float &pairwiseError, const double &elapsed);

int main(const int argc, const char* argv[]) {
    argh::parser argParser;
    argParser.parse(argc, argv, argh::parser::SINGLE_DASH_IS_MULTIFLAG);

    // check if help should be shown
    if (argc <= 1 || argParser[HELP.getArgument()] || !argParser(ROBOT_CONF.getArgument()) || !argParser(ENDPOINT_CONF.getArgument())) {
        showHelp();
    }

    // check for flags
    DEBUG.value = argParser["d"];

    // load robot from file
    std::vector<ArmSegment> arms;
    argParser(ROBOT_CONF.getArgument()) >> ROBOT_CONF.value;
    loadRobot(ROBOT_CONF.value, arms);
    Robot ROBOT(arms);

    // load endpoint from file
    std::vector<float> desiredEndpoint;
    argParser(ENDPOINT_CONF.getArgument()) >> ENDPOINT_CONF.value;
    loadEndpoint(ENDPOINT_CONF.value, desiredEndpoint);

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

    DifferentialEvolutionAlgorithm DE_algorithm(ROBOT,
                                      desiredEndpoint,
                                      POPULATION_SIZE.value,
                                      CROSSOVER_RATE.value,
                                      MUTATION_FACTOR.value,
                                      MAX_GENERATION.value,
                                      CNT_PROBE_LIMIT.value,
                                      EPSILON_LIMIT.value,
                                      SEED.value);

    startExperiment(DE_algorithm);

    return 0;
}

void startExperiment(DifferentialEvolutionAlgorithm algorithm) {
    int successfulRuns = 0;

    Result resultSum(0,0,0, std::vector<float>());
    Result best(0, std::numeric_limits<float>::max(), 0, std::vector<float>());

    for (int n = 0; n < NUMBER_OF_RUNS.value; n++) {
        std::cout << "======================== Run: " << n + 1 << " ========================" << std::endl;
        Result result = algorithm.begin(n);
        result.print();

        resultSum = resultSum + result;

        if(result.error < EPSILON_LIMIT.value) successfulRuns++;
        if(result.error < best.error) {
            best = result;
        }
    }

    std::cout << "===================== Success rate =====================" << std::endl;
    std::cout << "> value: " << ((float) successfulRuns / (float) NUMBER_OF_RUNS.value)*100 << "%" << std::endl;

    std::cout << "======================= Average ========================" << std::endl;
    resultSum.getAverage(NUMBER_OF_RUNS.value);
    std::cout << "========================= Best =========================" << std::endl;
    best.print();
}

void showHelp() {

    std::cout << "============== <HELP> ==============" << std::endl;
    std::cout << "Mandatory arguments are marked with *" << std::endl;
    std::cout << "Defaults (if any) are listed in brackets []" << std::endl << std::endl;

    std::cout << "Flags:" << std::endl;
    std::cout << "\t" << DEBUG.getArgument()            << " ~ " <<   DEBUG.getDescription()  << std::endl;

    std::cout << "Files:" << std::endl;
    std::cout << "\t* " << ROBOT_CONF.getArgument()            << " ~ " <<   ROBOT_CONF.getDescription() << std::endl;
    std::cout << "\t* " << ENDPOINT_CONF.getArgument()         << " ~ " <<   ENDPOINT_CONF.getDescription() << std::endl;

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
    exit(0);
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