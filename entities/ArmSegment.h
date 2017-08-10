//
// Created by timi on 7.8.2017.
//

#ifndef GENERALIZED_ROBOT_ARM_H
#define GENERALIZED_ROBOT_ARM_H


#include <vector>
#include "../utils/MatricesUtils.h"

class ArmSegment {

public: enum RotationOption { X_AXIS, Y_AXIS, Z_AXIS, NONE };

private:

    float maxLength;
    float minLength;

    float maxAngle;
    float minAngle;

    float error;

    RotationOption rotationOption;

public:

    ArmSegment(float maxLength, float minLength, float maxAngle, float minAngle, float error,  RotationOption rotationOption);
    ArmSegment(float length, float angle, float error, RotationOption rotationOption);

    std::vector<std::vector<float>> getRotationMatrix(const float angle);
    std::vector<std::vector<float>> getTranslationMatrix(const float length);
    std::vector<std::vector<float>> getTransformationMatrix(const float angle, const float length);

    float getMaxLength() const;

    float getMinLength() const;

    float getMaxAngle() const;

    float getMinAngle() const;

    float getError() const;
};

#endif //GENERALIZED_ROBOT_ARM_H