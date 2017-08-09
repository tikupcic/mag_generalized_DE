cmake_minimum_required(VERSION 3.6)
project(mag_generalized_DE)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        differential_evolution/OptimizedDE.cpp
        differential_evolution/OptimizedDE.h
        entities/Arm.cpp
        entities/Arm.h
        entities/Robot.cpp
        entities/Robot.h
        utils/MatricesUtils.cpp
        utils/MatricesUtils.h
        utils/Trigonometry.cpp
        utils/Trigonometry.h
        utils/VectorOperations.cpp
        utils/VectorOperations.h
        utils/TestingMatricesUtils.cpp
        utils/TestingMatricesUtils.h
        main.cpp)

add_executable(mag_generalized_DE ${SOURCE_FILES})