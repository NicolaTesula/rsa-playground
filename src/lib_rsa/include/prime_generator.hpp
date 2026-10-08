#pragma once

#include <tuple>
// In prime_generator.hpp
#include <functional>
#include <string>

using LogCallback = std::function<void(const std::string&)>;
std::tuple<std::uint32_t, std::uint32_t> generate_primes(const std::uint8_t bit_width, LogCallback log = nullptr);