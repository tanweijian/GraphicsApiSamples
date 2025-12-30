#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

typedef unsigned char byte; // Unsigned 8 bits

// Unsigned base types
typedef unsigned char uint8; // 8-bit  unsigned
typedef unsigned short uint16; // 16-bit unsigned
typedef unsigned int uint32; // 32-bit unsigned
typedef unsigned long long uint64; // 64-bit unsigned

// Signed base types
typedef signed char int8; // 8-bit  signed
typedef signed short int16; // 16-bit signed
typedef signed int int32; // 32-bit signed
typedef long long int64; // 64-bit signed

using String = std::string;
using StringView = std::string_view;