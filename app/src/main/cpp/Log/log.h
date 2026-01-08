#ifndef DOBBY_PROJECT_LOG_H
#define DOBBY_PROJECT_LOG_H

#include <android/log.h>  // 包含 Android NDK 的 log.h 文件

#endif //DOBBY_PROJECT_LOG_H

#include <stdarg.h>  // 用于支持可变参数宏

// 定义日志级别
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

// 控制日志输出的宏开关（如果为 1，则启用日志；如果为 0，则禁用日志）
#define ENABLE_LOGGING 1

// 当前日志级别
#define CURRENT_LOG_LEVEL LOG_LEVEL_INFO

// 宏定义：输出日志
#if ENABLE_LOGGING

#define LOG(level, fmt, ...) \
        do { \
            if (level >= CURRENT_LOG_LEVEL) { \
                int priority = ANDROID_LOG_INFO; \
                if (level == LOG_LEVEL_INFO) priority = ANDROID_LOG_INFO; \
                else if (level == LOG_LEVEL_WARN) priority = ANDROID_LOG_WARN; \
                else if (level == LOG_LEVEL_ERROR) priority = ANDROID_LOG_ERROR; \
                __android_log_print(priority, "[SFK]", "%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
            } \
        } while (0)

#else
#define LOG(level, fmt, ...)  // 如果禁用日志，则宏什么都不做
#endif
