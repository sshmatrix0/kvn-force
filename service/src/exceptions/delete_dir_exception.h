#pragma once
#include <exception>
#include <string>
#include "abstract_exception.h"

class DeleteDirException : public AbstractException {
public:
    explicit DeleteDirException(const std::string &dirPath) : dirPath(
        "Error to delete dir: " + dirPath + ", stacktrace: \n" + stacktrace) {
    }

    const char *what() const throw() override {
        return dirPath.c_str();
    }

private:
    std::string dirPath;
};
