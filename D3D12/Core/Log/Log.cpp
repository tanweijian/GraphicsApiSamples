#include "Log.h"
#include <spdlog/async.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

Logger::Logger()
{
    const spdlog::sinks_init_list sinks =
    {
        std::make_shared<spdlog::sinks::msvc_sink_mt>(),
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/log.log", true)
    };

    spdlog::init_thread_pool(8192, 1);

    _logger = std::make_shared<spdlog::async_logger>("logger", sinks.begin(), sinks.end(), spdlog::thread_pool());

    _logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%L] %v");
    _logger->set_level(spdlog::level::trace);
    _logger->flush_on(spdlog::level::err);

    spdlog::register_logger(_logger);
    spdlog::flush_every(std::chrono::seconds(1));
}

Logger::~Logger()
{
    _logger->flush();
    spdlog::drop_all();
}