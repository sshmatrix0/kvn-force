#pragma once
#include <string>
#include <cpptrace/cpptrace.hpp>
#include <QtLogging>

enum class LogLevel : int {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};

class Log {
public:
    bool isDebugEnabled() {
        return (int) level >= 1;
    }

    bool isTraceEnabled() {
        return (int) level >= 0;
    }

    void setLogLevel(LogLevel level) {
        this->level = level;
    }

    void trace(std::string message) {
        auto m = "[" + getPrefix() + "] " + message;
        qDebug("%s", m.c_str());
    }

    void debug(std::string message) {
        auto m = "[" + getPrefix() + "] " + message;
        qDebug("%s", m.c_str());
    }

    void info(std::string message) {
        auto m = "[" + getPrefix() + "] " + message;
        qInfo("%s", m.c_str());
    }

    void warn(std::string message) {
        auto m = "[" + getPrefix() + "] " + message;
        qWarning("%s", m.c_str());
    }

    void error(std::string message, const std::exception &ex) {
        auto m = "[" + getPrefix() + "] " + message + ", ex: " + ex.what();
        qCritical("%s", m.c_str());
    }

    void fatal(std::string message) {
#ifdef QT_DEBUG
        auto m = getPrefix() + " " + message
                 + "stack trace: " + cpptrace::generate_trace().to_string();
        qFatal("%s", m.c_str());
#else
        auto m = getPrefix() + " " + message;
        qFatal("%s", m.c_str());
#endif
    }

private:
    std::string getPrefix() {
        std::string retval;
#ifdef QT_DEBUG
        auto st = cpptrace::generate_trace();
        auto frames = st.frames;
        auto frame0 = frames.at(0);
        if (frame0.filename.find("logger.h") == std::string::npos) {
            return "";
        }
        auto frame = frames.at(2);
        auto fileName = frame.filename;
        std::string line = "";
        std::string col = "";
        if (frame.line.has_value()) {
            line = std::to_string(frame.line.value());
        }
        if (frame.column.has_value()) {
            col = std::to_string(frame.column.value());
        }
        retval = fileName + "(" + line + ":" + col + ")";
        if (retval.length() > 30) {
            retval = "..." + retval.substr(retval.length() - 30);
        }
        retval = "[" + retval + "]";
#endif
        return retval;
    }

    LogLevel level = LogLevel::ERROR;
};

extern Log Logger;
