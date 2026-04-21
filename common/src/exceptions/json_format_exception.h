#pragma once
#include <exception>
#include <string>
#include "abstract_exception.h"

class JsonFormatException : public AbstractException {
public:
    explicit JsonFormatException(const std::string &message) : message(message + ", stacktrace: \n" + stacktrace) {
    }

    const char *what() const throw() override {
        return message.c_str();
    }

private:
    std::string message;
};
