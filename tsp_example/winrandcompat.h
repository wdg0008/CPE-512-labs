#ifndef WINRANDCOMPAT_H
#define WINRANDCOMPAT_H

#include <cstdint>

// Reproduce the srand48()/drand48() sequence on Windows.
// Requires C++17 or later for the inline variable.

namespace drand48_compat {

inline std::uint64_t state = 0x1234ABCD330EULL;

inline constexpr std::uint64_t mask = (1ULL << 48) - 1;
inline constexpr std::uint64_t multiplier = 0x5DEECE66DULL;
inline constexpr std::uint64_t increment = 0xBULL;

} // namespace drand48_compat

extern "C" inline void srand48(long int seedval)
{
    drand48_compat::state =
        ((static_cast<std::uint64_t>(
              static_cast<std::uint32_t>(seedval))
          << 16)
         | 0x330EULL);
}

extern "C" inline double drand48(void)
{
    using namespace drand48_compat;

    state = (multiplier * state + increment) & mask;

    return static_cast<double>(state) / 281474976710656.0;
}

#endif