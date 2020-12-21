#pragma once

#include <stdint.h>
#include <stdio.h>

#define _SCOPED_PROFILER(file, line, string)                                   \
    ScopedProfiler profile##line(file, line, string);

/// \def PROFILE_FUNCTION
///
/// Insert a scoped profiler tagged with the pretty-function interpretation
/// of the parent function.
#define PROFILE_FUNCTION()                                                     \
    _SCOPED_PROFILER(__FILE__, __LINE__, __PRETTY_FUNCTION__)

class ScopedProfiler
{
public:
    explicit ScopedProfiler(const char* i_file,
                            uint32_t i_line,
                            const char* i_string)
      : m_file(i_file)
      , m_line(i_line)
      , m_string(i_string)
    {
        // Record start.
        clock_gettime(CLOCK_MONOTONIC, &m_start);
    }

    ~ScopedProfiler()
    {
        // Record stop.
        clock_gettime(CLOCK_MONOTONIC, &m_stop);

        // Compute duration in microseconds.
        timespec duration = _ComputeDuration(m_start, m_stop);
        uint32_t durationUs =
            (uint32_t)(duration.tv_sec * 1e6 + duration.tv_nsec / 1e3);

        printf("%s, %s:%u took %u ms\n", m_string, m_file, m_line, durationUs);
    }

    // Cannot be copied.
    ScopedProfiler(const ScopedProfiler& i_profile) = delete;
    ScopedProfiler& operator=(const ScopedProfiler& i_profile) = delete;

private:
    // Compute the timespec duration between \p i_start and \p i_stop.
    static timespec _ComputeDuration(const timespec& i_start,
                                     const timespec& i_stop)
    {
        timespec duration;
        if ((i_stop.tv_nsec - i_start.tv_nsec) < 0) {
            duration.tv_sec = i_stop.tv_sec - i_start.tv_sec - 1;
            duration.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec + 1e9;
        } else {
            duration.tv_sec = i_stop.tv_sec - i_start.tv_sec;
            duration.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec;
        }
        return duration;
    }

    const char* m_file = nullptr;
    uint32_t m_line = 0;
    const char* m_string = nullptr;
    timespec m_start = { 0, 0 };
    timespec m_stop = { 0, 0 };
};
