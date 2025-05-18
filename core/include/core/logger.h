#pragma once 
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace cheeky::core {
    class Logger {
        using inner_t = std::shared_ptr<spdlog::logger>;
    public:
        inline Logger(std::string_view name) {
            _inner = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>(std::string{name});
        }

        template <typename... Args>
        inline void debug(std::string_view name, Args... args) {
            _inner->debug(name, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void info(std::string_view name, Args... args) {
            _inner->info(name, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void warn(std::string_view name, Args... args) {
            _inner->warn(name, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void error(std::string_view name, Args... args) {
            _inner->error(name, std::forward<Args>(args)...);
        }

    private:
        inner_t _inner;
    };
}