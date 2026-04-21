#pragma once
#include <exception>
#include <string>
#include "abstract_exception.h"

class CopyConfigException : public AbstractException {
public:
    explicit CopyConfigException(const std::string &message) : message(message + ", stacktrace: \n" + stacktrace) {
    }

    const char *what() const throw() override {
        return message.c_str();
    }

private:
    std::string message;
};
