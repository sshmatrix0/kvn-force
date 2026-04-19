#pragma once
#include <exception>
#include <string>
#include "abstract_exception.h"

class CreateDirException : public AbstractException {
public:
    explicit CreateDirException(const std::string &dirPath) : dirPath(
        "Error to create dir: " + dirPath + ", stacktrace: \n" + stacktrace) {
    }

    const char *what() const throw() override {
        return dirPath.c_str();
    }

private:
    std::string dirPath;
};
