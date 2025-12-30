#pragma once

#include "Core/Types/BaseTypes.h"
#include "Core/Misc/Singleton.h"
#include <spdlog/spdlog.h>

#define LOG_T(...) Logger::Instance()->Trace(__VA_ARGS__)
#define LOG_D(...) Logger::Instance()->Debug(__VA_ARGS__)
#define LOG_I(...) Logger::Instance()->Info(__VA_ARGS__)
#define LOG_W(...) Logger::Instance()->Warn(__VA_ARGS__)
#define LOG_E(...) Logger::Instance()->Error(__VA_ARGS__)
#define LOG_F(...) Logger::Instance()->Fatal(__VA_ARGS__)

enum class LogType
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

class Logger : public Singleton<Logger>
{
public:
	Logger();
	virtual ~Logger();

private:
    std::shared_ptr<spdlog::logger> _logger;

public:
    template <typename T>
    void Trace(const T& msg)
    {
        _logger->trace(msg);
    }

    template <typename... Args>
    void Trace(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Debug(const T& msg)
    {
        _logger->debug(msg);
    }

    template <typename... Args>
    void Debug(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Info(const T& msg)
    {
        _logger->info(msg);
    }

    template <typename... Args>
    void Info(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Warn(const T& msg)
    {
        _logger->warn(msg);
    }

    template <typename... Args>
    void Warn(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Error(const T& msg)
    {
        _logger->error(msg);
    }

    template <typename... Args>
    void Error(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Fatal(const T& msg)
    {
        _logger->critical(msg);
        const String formatStr = fmt::format(msg);
        throw std::runtime_error(formatStr);
    }

    template <typename... Args>
    void Fatal(fmt::format_string<Args...> fmt, Args &&...args)
    {
        _logger->critical(fmt, std::forward<Args>(args)...);
        const String formatStr = fmt::format(fmt, std::forward<Args>(args)...);
        throw std::runtime_error(formatStr);
    }
};

