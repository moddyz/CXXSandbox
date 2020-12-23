#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sstream>
#include <cassert>

#define ASSERT(expr) assert(expr);

#define _SCOPED_PROFILER(file, line, string)                                   \
    ScopedProfiler profile##line(file, line, string);

/// \def PROFILE_FUNCTION
///
/// Insert a scoped profiler tagged with the pretty-function interpretation
/// of the parent function.
#define PROFILE_FUNCTION()                                                     \
    _SCOPED_PROFILER(__FILE__, __LINE__, __PRETTY_FUNCTION__)

/// \class ScopedProfiler
///
/// Prints the elapsed time of the object lifetime.
class ScopedProfiler
{
public:
    inline explicit ScopedProfiler(const char* i_file,
                                   uint32_t i_line,
                                   const char* i_string)
      : m_file(i_file)
      , m_line(i_line)
      , m_clientString(i_string)
    {
        // Record start.
        clock_gettime(CLOCK_MONOTONIC, &m_start);
    }

    inline ~ScopedProfiler()
    {
        // Record stop.
        clock_gettime(CLOCK_MONOTONIC, &m_stop);

        // Print elapsed time.
        uint32_t elapsedMs = ComputeElapsedMs();
        printf("%s, %s:%u took %u ms\n",
               m_clientString,
               m_file,
               m_line,
               elapsedMs);
    }

    // Cannot be copied.
    ScopedProfiler(const ScopedProfiler& i_profile) = delete;
    ScopedProfiler& operator=(const ScopedProfiler& i_profile) = delete;

    // Compute the elapsed time in milliseconds (us).
    inline uint32_t ComputeElapsedMs() const
    {
        // Compute elapsed in microseconds.
        timespec elapsed = _ComputeElapsed(m_start, m_stop);
        return (uint32_t)(elapsed.tv_sec * 1e6 + elapsed.tv_nsec / 1e3);
    }

private:
    // Compute the timespec elapsed between \p i_start and \p i_stop.
    static timespec _ComputeElapsed(const timespec& i_start,
                                    const timespec& i_stop)
    {
        timespec elapsed;
        if ((i_stop.tv_nsec - i_start.tv_nsec) < 0) {
            elapsed.tv_sec = i_stop.tv_sec - i_start.tv_sec - 1;
            elapsed.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec + 1e9;
        } else {
            elapsed.tv_sec = i_stop.tv_sec - i_start.tv_sec;
            elapsed.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec;
        }
        return elapsed;
    }

    const char* m_file = nullptr;
    uint32_t m_line = 0;
    const char* m_clientString = nullptr;
    timespec m_start = { 0, 0 };
    timespec m_stop = { 0, 0 };
};

/// Convert a string into a specified value type.
///
/// \tparam The desired value type.
/// \param string String token.
///
/// \return The converted value.
template< typename T >
T DeserializeString( const char* string )
{
    std::stringstream ss;
    ss << string;
    T value;
    ss >> value;
    return value;
}

