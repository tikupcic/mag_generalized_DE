//
// Created by timi on 8.8.2017.
//

#include "Robot.h"

Robot::Robot(const std::vector<Arm> &arms) : arms(arms) {}

int Robot::getDimension() const {
    return arms.size()*2;
}

std::vector<Arm> &Robot::getArms() {
    return arms;
}
