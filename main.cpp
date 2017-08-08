
#include <iostream>
#include "entities/Arm.h"
#include "entities/Robot.h"
#include "differential_evolution/OptimizedDE.h"

int main() {

    Arm arm1   (200,    300,     160,    -160,   Arm::RotationOption::Y_AXIS);
    Arm arm2   (200,    300,     125,    -125,   Arm::RotationOption::Z_AXIS);
    Arm arm3   (200,    300,     135,    -135,   Arm::RotationOption::Z_AXIS);
    Arm arm4   (50,    50,     135,    -135,    Arm::RotationOption::Z_AXIS);
    Arm arm5   (50,    50,     135,    -135,    Arm::RotationOption::Z_AXIS);

    std::vector<Arm> arms{arm1,arm2,arm3,arm4,arm5};
    Robot robot(arms);

    std::vector<float> endPoint{350, 350, 350};

    OptimizedDE algorithm(robot, 10, 1000, .5f, .9f);
    algorithm.begin(endPoint);

    std::string raw = "";
    for (auto &arm : robot.getArms()) {
        std::cout << std::endl << "ARM " << arm.getIndex() + 1 <<":\t";
        std::cout << arm.getAngle() << "° / " << arm.getLength() << "mm";

        raw += std::to_string(arm.getAngle()) + "f," + std::to_string(arm.getLength()) + "f,";
    }
    std::cout << std::endl << std::endl << "raw data: " << raw << std::endl;

    return 0;
}