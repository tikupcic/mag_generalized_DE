
#include <iostream>
#include "entities/ArmSegment.h"
#include "entities/Robot.h"
#include "differential_evolution/DifferentialEvolutionAlgorithm.h"

int main() {

    const bool considerErrors = false;
    std::vector<float> endPoint{350, 350, 350};

    ArmSegment segment1   (600,    320,   .01f,   ArmSegment::RotationOption::Y_AXIS);
    ArmSegment segment2   (400,    250,   .01f,   ArmSegment::RotationOption::Z_AXIS);
    ArmSegment segment3   (400,    400,     135,    -135,   .01f,   ArmSegment::RotationOption::Z_AXIS);

    std::vector<ArmSegment> arms{segment1,segment2,segment3};
    Robot robot(arms);

    DifferentialEvolutionAlgorithm algorithm(robot, 100, 100, .5f, .9f, considerErrors);
    std::vector<float> result = algorithm.begin(endPoint);

    std::cout << "====================================" << std::endl;
    printf("> Overall best w/o error: %.2f mm\n", DifferentialEvolutionAlgorithm::fitnessFunction(result));
    printf("> Overall best w/  error: %.2f mm\n", DifferentialEvolutionAlgorithm::pairwiseFitnessFunction(result));

    std::string raw = "";
    for (int i = 0; i < result.size(); i+=2) {
        printf("\n Segment %d : %.2f° / %.2f mm", i/2 + 1, result[i], result[i+1]);

        raw += std::to_string(result[i]) + "f," + std::to_string(result[i+1]) + "f,";
    }
    std::cout << std::endl << std::endl << "raw data: " << raw << std::endl;

    return 0;
}