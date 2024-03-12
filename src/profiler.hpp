//
// Created by hackerman on 7/09/23.
//
#pragma once


namespace profiler
{
template<typename callable_t, typename ratio_t = std::chrono::milliseconds>
[[nodiscard]] double benchmark(callable_t&& func) requires std::invocable<callable_t>
{
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double, typename ratio_t::period>>( end - start ).count();
}
}   // namespace profiler