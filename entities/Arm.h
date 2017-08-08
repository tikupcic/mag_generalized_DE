//
// Created by timi on 7.8.2017.
//

#ifndef GENERALIZED_ROBOT_ARM_H
#define GENERALIZED_ROBOT_ARM_H


#include <vector>
#include "../utils/MatricesUtils.h"

class Arm {

public: enum RotationOption { X_AXIS, Y_AXIS, Z_AXIS, NONE };

private:

    int index;

    float maxLength;
    float minLength;
    float length;

    float maxAngle;
    float minAngle;
    float angle;

    RotationOption rotationOption;
    std::vector<std::vector<float>> transformationMatrix;

public:
    static int counter;

    Arm(float maxLength, float minLength, float maxAngle, float minAngle, RotationOption rotationOption);

    std::vector<std::vector<float>> getRotationMatrix(const float angle);
    std::vector<std::vector<float>> getTranslationMatrix(const float length);
    std::vector<std::vector<float>> getTransformationMatrix(const float angle, const float length);

    int getIndex() const;

    void setIndex(int index);

    float getMaxLength() const;

    float getMinLength() const;

    float getMaxAngle() const;

    float getMinAngle() const;

    float getLength() const;

    void setLength(float length);

    float getAngle() const;

    void setAngle(float angle);
};

#endif //GENERALIZED_ROBOT_ARM_H