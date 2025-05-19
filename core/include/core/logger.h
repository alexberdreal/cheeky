#pragma once 
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

/*
    Logger class is a wrapper for async stdout logger, used for stand-alone objects

    BaseLogger is a convenience class for inheritance, so you can do:

    struct A : BaseLogger {
        A() : BaseLogger("A") { _logger.info(...); }
    };

    Important: in order to make compilation faster, to specify log level as a macro and to support disabling a spdlog 
    (thread, formatting, event enqueing ...) single header version is not used.
    A problem with cmake FetchContent is that when spdlog sources are compiled, cheeky's macro are not visible,
    so there is no way to specify log level with SPDLOG_ACTIVE_LEVEL
*/

namespace cheeky::core {
    class Logger {
        using inner_t = std::shared_ptr<spdlog::logger>;
    public:
        inline Logger(std::string_view name) {
            #if LOGLEVEL != SPDLOG_LEVEL_OFF
                _inner = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>(std::string{name});
            #endif
        }    

        template <typename... Args>
        inline void debug(std::string_view fmt_str, Args... args) {
            #if LOGLEVEL != SPDLOG_LEVEL_OFF
                _inner->debug(fmt_str, std::forward<Args>(args)...);
            #endif
        }
    
        template <typename... Args>
        inline void info(std::string_view fmt_str, Args... args) {
            #if LOGLEVEL != SPDLOG_LEVEL_OFF
                _inner->info(fmt_str, std::forward<Args>(args)...);
            #endif
        }
    
        template <typename... Args>
        inline void warn(std::string_view fmt_str, Args... args) {
            #if LOGLEVEL != SPDLOG_LEVEL_OFF
                _inner->warn(fmt_str, std::forward<Args>(args)...);
            #endif
        }
    
        template <typename... Args>
        inline void error(std::string_view fmt_str, Args... args) {
            #if LOGLEVEL != SPDLOG_LEVEL_OFF
                _inner->error(fmt_str, std::forward<Args>(args)...);
            #endif
        }
    private:
        inner_t _inner;
    };

    class BaseLogger {
    protected:
        inline BaseLogger(std::string_view name) : _logger(name) {}

        Logger _logger;
    };
}