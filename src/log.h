#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>

typedef enum {
    LLTrace = 0,
    LLInfo = 1,
    LLWarn = 2,
    LLError = 3,
} LogLevel;

#define __FILENAME__                                                           \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define CLEAR "\033[0m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GRAY "\033[90m"

extern LogLevel __global_log_level;

#define SET_LOG_LEVEL(level) (__global_log_level = (level))
#define GET_LOG_LEVEL() (__global_log_level)

#define __LOG_PRINT(file, level, format, ...)                                  \
    do {                                                                       \
        fprintf(file,                                                          \
                "%s " GRAY "[" BLUE "%s" GRAY "@" BLUE "%s" GRAY ":" BLUE      \
                "%d" GRAY "] " CLEAR,                                          \
                level, __func__, __FILENAME__, __LINE__);                      \
        fprintf(file, format, ##__VA_ARGS__);                                  \
        fprintf(file, "\n");                                                   \
    } while (0)

#define TRACE(format, ...)                                                     \
    do {                                                                       \
        if (__global_log_level <= LLTrace) {                                   \
            __LOG_PRINT(stdout,                                                \
                        GRAY "[" CYAN BOLD "TRACE" CLEAR GRAY "]" CLEAR,       \
                        format, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#define INFO(format, ...)                                                      \
    do {                                                                       \
        if (__global_log_level <= LLInfo) {                                    \
            __LOG_PRINT(stdout,                                                \
                        GRAY "[" GREEN BOLD "INFO" CLEAR GRAY "] " CLEAR,      \
                        format, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#define WARN(format, ...)                                                      \
    do {                                                                       \
        if (__global_log_level <= LLWarn) {                                    \
            __LOG_PRINT(stderr,                                                \
                        GRAY "[" YELLOW BOLD "WARN" CLEAR GRAY "] " CLEAR,     \
                        format, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#define ERROR(format, ...)                                                     \
    do {                                                                       \
        if (__global_log_level <= LLError) {                                   \
            __LOG_PRINT(stderr,                                                \
                        GRAY "[" RED BOLD "ERROR" CLEAR GRAY "]" CLEAR,        \
                        format, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#endif
