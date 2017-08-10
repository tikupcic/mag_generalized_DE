//
// Created by timi on 7.8.2017.
//

#include "ArmSegment.h"

ArmSegment::ArmSegment(float maxLength, float minLength, float maxAngle, float minAngle, float error,
                       ArmSegment::RotationOption rotationOption)
        : maxLength(maxLength),
          minLength(minLength),
          maxAngle(maxAngle),
          minAngle(minAngle),
          error(error),
          rotationOption(rotationOption) {};

ArmSegment::ArmSegment(float length, float angle, float error, ArmSegment::RotationOption rotationOption) :
        ArmSegment(length, length, angle / 2, angle / -2, error, rotationOption) {}

std::vector<std::vector<float>> ArmSegment::getRotationMatrix(const float angle) {
    switch (rotationOption) {
        case X_AXIS:
            return MatricesUtils::getRotationAboutX(angle);
        case Y_AXIS:
            return MatricesUtils::getRotationAboutY(angle);
        case Z_AXIS:
            return MatricesUtils::getRotationAboutZ(angle);
        case NONE:
            return MatricesUtils::getIdentityMatrix();
    }
}

std::vector<std::vector<float>> ArmSegment::getTranslationMatrix(const float length) {
    return MatricesUtils::getTranslationY(length);
}

std::vector<std::vector<float>> ArmSegment::getTransformationMatrix(const float angle, const float length) {
    return MatricesUtils::multiply(getRotationMatrix(angle), getTranslationMatrix(length));
}

float ArmSegment::getMaxLength() const {
    return maxLength;
}

float ArmSegment::getMinLength() const {
    return minLength;
}

float ArmSegment::getMaxAngle() const {
    return maxAngle;
}

float ArmSegment::getMinAngle() const {
    return minAngle;
}

float ArmSegment::getError() const {
    return error;
}