//
// Created by timi on 7.8.2017.
//

#include "Arm.h"

int Arm::counter = 0;

Arm::Arm(float maxLength, float minLength, float maxAngle, float minAngle, Arm::RotationOption rotationOption)
        : maxLength(maxLength), minLength(minLength), maxAngle(maxAngle), minAngle(minAngle),
          rotationOption(rotationOption) {
    setIndex(Arm::counter++);
}

std::vector<std::vector<float>> Arm::getRotationMatrix(const float angle) {
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

std::vector<std::vector<float>> Arm::getTranslationMatrix(const float length) {
    return MatricesUtils::getTranslationY(length);
}

std::vector<std::vector<float>> Arm::getTransformationMatrix(const float angle, const float length) {
    return MatricesUtils::multiply(getRotationMatrix(angle), getTranslationMatrix(length));
}

int Arm::getIndex() const {
    return index;
}

void Arm::setIndex(int index) {
    Arm::index = index;
}

float Arm::getMaxLength() const {
    return maxLength;
}

float Arm::getMinLength() const {
    return minLength;
}

float Arm::getMaxAngle() const {
    return maxAngle;
}

float Arm::getMinAngle() const {
    return minAngle;
}

float Arm::getLength() const {
    return length;
}

void Arm::setLength(float length) {
    Arm::length = length;
}

float Arm::getAngle() const {
    return angle;
}

void Arm::setAngle(float angle) {
    Arm::angle = angle;
}