//
// Created by hackerman on 6/09/23.
//

#pragma once


namespace logger
{
    static constexpr std::string_view COLOR_CLR = "\033[0m";
    static constexpr std::string_view COLOR_GREEN = "\033[32m";
    static constexpr std::string_view COLOR_YELLOW = "\033[33m";
    static constexpr std::string_view COLOR_RED = "\033[31m";
    static constexpr std::string_view FORMATTED_MSG = "File: {}\nFunction: {}\nLine: {}\nMessage: {}";

    template<typename string_t>
    void log_msg(std::string_view color, std::string_view type, string_t&& msg)
    {
        std::printf("\n%s\n%s\n%s%s\n", color.data(), type.data(), msg.c_str(), COLOR_CLR.data());
    };
    template<typename string_t>
    void log_info(string_t&& msg)
    {
        log_msg(COLOR_GREEN, "INFO", std::forward<string_t>(msg));
    };
    template<typename string_t>
    void log_warn(std::string_view file, std::string_view function, int32_t line, string_t&& msg)
    {
        log_msg(COLOR_YELLOW, "WARNING", std::format(FORMATTED_MSG.data(), file, function, line, std::forward<string_t>(msg)));
    };
    template<typename string_t>
    void log_err(std::string_view file, std::string_view function, int32_t line, string_t&& msg)
    {
        log_msg(COLOR_RED, "ERROR", std::format(FORMATTED_MSG.data(), file, function, line, std::forward<string_t>(msg)));
    };
    template<typename string_t>
    void log_fat(std::string_view file, std::string_view function, int32_t line, string_t&& msg)
    {
        log_msg(COLOR_RED, "UNRECOVERABLE ERROR", std::format(FORMATTED_MSG.data(), file, function, line, std::forward<string_t>(msg)));
        throw std::runtime_error(std::string{"Fatal runtime error. See log. Aborting.."});
    };
    template<typename string_t>
    void log_ass(std::string_view file, std::string_view function, int32_t line, string_t&& msg)
    {
        log_msg(COLOR_RED, "ASSERTION FAILURE", std::format(FORMATTED_MSG.data(), file, function, line, std::forward<string_t>(msg)));
    };
}
