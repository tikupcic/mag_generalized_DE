
#include <iostream>
#include "entities/ArmSegment.h"
#include "entities/Robot.h"
#include "differential_evolution/DifferentialEvolutionAlgorithm.h"

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

int main() {

    const int NUMBER_OF_RUNS = 10;
    float sumElapsed = 0;
    float sumError = 0;
    float sumPairWiseError = 0;

    std::vector<float> endPoint{350, 350, 350};

    ArmSegment segment1(600, 320, .01f, ArmSegment::RotationOption::Y_AXIS);
    ArmSegment segment2(400, 250, .01f, ArmSegment::RotationOption::Z_AXIS);
    ArmSegment segment3(400, 400, 135, -135, .01f, ArmSegment::RotationOption::Z_AXIS);

    std::vector<ArmSegment> arms{segment1, segment2, segment3};
    Robot robot(arms);

    DifferentialEvolutionAlgorithm algorithm(robot, 20, 500, .5f, .9f, 1, 100, false);

    for (int n = 0; n < NUMBER_OF_RUNS; n++) {
        clock_t begin = clock();
        std::vector<float> result = algorithm.begin(endPoint);
        clock_t end = clock();

        std::cout << "========================== Run: " << n + 1 << " ==========================" << std::endl;

        float pairwiseError = algorithm.pairwiseFitnessFunction(result);
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        float error = algorithm.fitnessFunction(result);

        sumPairWiseError += pairwiseError;
        sumElapsed += elapsed_secs;
        sumError += error;

        printf("> Run best w/o error: %.2f mm\n", error);
        printf("> Run best w/  error: %.2f mm\n", pairwiseError);
        printf("> Run elapsed time:   %.2f sec\n", elapsed_secs);

        std::string raw = "";
        for (int i = 0; i < result.size(); i += 2) {
            printf("\n Segment %d : %.2f° / %.2f mm", i / 2 + 1, result[i], result[i + 1]);

            raw += std::to_string(result[i]) + "f," + std::to_string(result[i + 1]) + "f,";
        }
        std::cout << std::endl << std::endl << "raw data: " << raw << std::endl;
    }

    std::cout << "======================= OVERALL RESULTS =======================" << std::endl;

    printf("> Overall best w/o error: %.2f mm\n", sumError / NUMBER_OF_RUNS);
    printf("> Overall best w/  error: %.2f mm\n", sumPairWiseError / NUMBER_OF_RUNS);
    printf("> Elapsed time:           %.2f sec\n", sumElapsed / NUMBER_OF_RUNS);

    return 0;
}