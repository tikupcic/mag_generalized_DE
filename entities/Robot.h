//
// Created by timi on 8.8.2017.
//

#ifndef GENERALIZED_ROBOT_ROBOT_H
#define GENERALIZED_ROBOT_ROBOT_H


#include "ArmSegment.h"

class Robot {

private:
    std::vector<ArmSegment> arms;

public:
    Robot(const std::vector<ArmSegment> &arms);

    int getDimension() const;

    std::vector<ArmSegment> &getArms();
};


#endif //GENERALIZED_ROBOT_ROBOT_H
