//
// Created by Timi on 14. 08. 2017.
//

#ifndef MAG_GENERALIZED_DE_ARGUMENT_H
#define MAG_GENERALIZED_DE_ARGUMENT_H

#include <string>
#include <sstream>

template<typename T>
class Argument {
private:
    const std::string argument;
    const std::string description;
    T defValue;

public:
    T value;

    Argument(const std::string &argument, const std::string &description) : argument(argument),
                                                                            description(description) {}

    Argument(const std::string &argument, const std::string &description, const T defValue) : argument(argument),
                                                                                              description(description),
                                                                                              defValue(defValue) {}

    const std::string &getArgument() const {
        return argument;
    }

    const std::string &getDescription() const {
        return description;
    }

    T getDefValue() const {
        return defValue;
    }

    std::string defToString() const {
        std::stringstream ss;
        ss << " [" << getDefValue() << "] ";
        return ss.str();
    }
};

#endif //MAG_GENERALIZED_DE_ARGUMENT_H