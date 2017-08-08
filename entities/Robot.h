//
// Created by timi on 8.8.2017.
//

#ifndef GENERALIZED_ROBOT_ROBOT_H
#define GENERALIZED_ROBOT_ROBOT_H


#include "Arm.h"

class Robot {

private:
    std::vector<Arm> arms;

public:
    Robot(const std::vector<Arm> &arms);

    int getDimension() const;

    std::vector<Arm> &getArms();
};


#endif //GENERALIZED_ROBOT_ROBOT_H
