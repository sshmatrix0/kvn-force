//
// Created by ksp on 26.03.2026.
//
#pragma once
#include <string>
#include "abstract_exception.h"

class ProcessFailedException : public AbstractException {
public:
    explicit ProcessFailedException(const std::string &message) : message(message + ", stacktrace: \n" + stacktrace) {
    }

    const char *what() const throw() override {
        return message.c_str();
    }

private:
    std::string message;
};