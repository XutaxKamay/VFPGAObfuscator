#ifndef VFPGA_OBFUSCATOR_LIBRARY_BIT_H
#define VFPGA_OBFUSCATOR_LIBRARY_BIT_H

#include "StandardHeaders.h"

namespace VFPGAObfuscatorLibrary
{
    struct Bit
    {
        constexpr Bit& operator^=(const Bit& other);
        constexpr Bit& operator&=(const Bit& other);
        constexpr Bit& operator|=(const Bit& other);
        constexpr Bit operator~() const;
        constexpr Bit operator^(const Bit& other) const;
        constexpr Bit operator&(const Bit& other) const;
        constexpr Bit operator|(const Bit& other) const;
        constexpr bool operator==(const Bit& other) const;
        constexpr bool operator!=(const Bit& other) const;

        constexpr operator std::uint_fast8_t() const;

        constexpr void Set(std::uint_fast8_t state);

        std::uint_fast8_t value;
    };
}

constexpr VFPGAObfuscatorLibrary::Bit& VFPGAObfuscatorLibrary::Bit::
  operator^=(const Bit& other)
{
    Set(value ^ other.value);
    return *this;
}

constexpr VFPGAObfuscatorLibrary::Bit& VFPGAObfuscatorLibrary::Bit::
  operator&=(const Bit& other)
{
    Set(value & other.value);
    return *this;
}

constexpr VFPGAObfuscatorLibrary::Bit& VFPGAObfuscatorLibrary::Bit::
  operator|=(const Bit& other)
{
    Set(value | other.value);
    return *this;
}

constexpr VFPGAObfuscatorLibrary::Bit VFPGAObfuscatorLibrary::Bit::
  operator~() const
{
    Bit ret;
    ret.Set(~value);
    return ret;
}

constexpr VFPGAObfuscatorLibrary::Bit VFPGAObfuscatorLibrary::Bit::
  operator^(const Bit& other) const
{
    auto copy = *this;
    copy.Set(value ^ other.value);
    return copy;
}

constexpr VFPGAObfuscatorLibrary::Bit VFPGAObfuscatorLibrary::Bit::
  operator&(const Bit& other) const
{
    auto copy = *this;
    copy.Set(value & other.value);
    return copy;
}

constexpr VFPGAObfuscatorLibrary::Bit VFPGAObfuscatorLibrary::Bit::
  operator|(const Bit& other) const
{
    auto copy = *this;
    copy.Set(value | other.value);
    return copy;
}

constexpr bool VFPGAObfuscatorLibrary::Bit::operator==(
  const Bit& other) const
{
    return value == other.value;
}

constexpr bool VFPGAObfuscatorLibrary::Bit::operator!=(
  const Bit& other) const
{
    return value != other.value;
}

constexpr VFPGAObfuscatorLibrary::Bit::operator std::uint_fast8_t() const
{
    return value;
}

constexpr void VFPGAObfuscatorLibrary::Bit::Set(std::uint_fast8_t state)
{
    value = state & 1;
}

consteval VFPGAObfuscatorLibrary::Bit operator""_vfpga_obf_lib_bit(
  unsigned long long i)
{
    return { static_cast<std::uint_fast8_t>(i) };
}

#endif
